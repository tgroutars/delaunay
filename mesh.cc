#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <algorithm>

#include "mesh.h"
#include "robust_predicates.h"

#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MAX(a,b) ((a) > (b) ? (a) : (b))

using namespace std;

Mesh::~Mesh() {
  if (vertices_ != nullptr) {
    if (vertices_[0] != nullptr) {
      delete[] vertices_[0];
    }
    delete[] vertices_;
  }
  if (edges_ != nullptr) {
    delete[] edges_;
  }
}

void Mesh::Triangulate(const char* filename) {
  // Initialize robust Predicates
  robust_predicates::exactinit();
  ReadFile(filename);
  Shake();
  SortVertices();
  DelaunayDC(0, size_ - 1);
  CountEdges();
}

void Mesh::DelaunayDC(int start, int end) {
  int size = end - start + 1;
  if (size < 2) {
    return;
  }
  if (size == 2) {
    BuildEdge(start, end);
    return;
  }
  int middle = (start + end) / 2;
  if (size == 3) {
    double area = robust_predicates::orient2d(vertices_[start], vertices_[middle], vertices_[end]);
    if (area > 0) {
      BuildEdge(start, middle);
      BuildEdge(middle, end);
      BuildEdge(end, start);
    } else {
      BuildEdge(start, end);
      BuildEdge(end, middle);
      BuildEdge(middle, start);
    }
    return;
  }
  DelaunayDC(start, middle);
  DelaunayDC(middle + 1, end);
  MergeMeshes(start, middle, end);
}

int Mesh::RightCandidate(int vl, int vr, int middle) {
  DoublyLinkedListElem<int> *candidate = edges_[vr].Predecessor(vl);
  if (candidate->data() < middle + 1)
    return -1;
  if (robust_predicates::orient2d(vertices_[vl], vertices_[vr], vertices_[candidate->data()]) <= 0.) {
    return -1;
  }
  DoublyLinkedListElem<int> *next_candidate = candidate->previous();
  if (next_candidate->data() <= middle + 1)
    return candidate->data();
  double in_circle = robust_predicates::isincircle(vertices_[vl], vertices_[vr], vertices_[candidate->data()],
                                                   vertices_[next_candidate->data()]);
  if (in_circle < 0) {
    return candidate->data();
  }

  DestroyEdge(vr, candidate->data());
  return RightCandidate(vl, vr, middle);
}

int Mesh::LeftCandidate(int vl, int vr, int middle) {
  DoublyLinkedListElem<int> *candidate = edges_[vl].Successor(vr);
  if (candidate->data() > middle)
    return -1;
  if (robust_predicates::orient2d(vertices_[vl], vertices_[vr], vertices_[candidate->data()]) <= 0.) {
    return -1;
  }

  DoublyLinkedListElem<int> *next_candidate = candidate->next();
  if (next_candidate->data() >= middle)
    return candidate->data();
  double in_circle = robust_predicates::isincircle(vertices_[vl], vertices_[vr], vertices_[candidate->data()],
                                                   vertices_[next_candidate->data()]);

  if (in_circle < 0) {
    return candidate->data();
  }
  DestroyEdge(vl, candidate->data());
  return LeftCandidate(vl, vr, middle);
}

void Mesh::MergeMeshes(int start, int middle, int end) {
  int lower_common_tangent[2];

  LowerCommonTangent(middle, lower_common_tangent);

  int vl = lower_common_tangent[0], vr = lower_common_tangent[1],
      vr1 = -1, vl1 = -1;

  BuildEdge(vl, vr);
  do {

    vr1 = RightCandidate(vl, vr, middle);
    vl1 = LeftCandidate(vl, vr, middle);

    if (vl1 == -1 && vr1 == -1)
      return;

    if (vl1 != -1 && vr1 != -1) {
      double in_circle = robust_predicates::isincircle(vertices_[vl], vertices_[vr], vertices_[vl1], vertices_[vr1]);
      if (in_circle <= 0) {
        vr1 = -1;
      }
    }

    if (vr1 == -1) {
      edges_[vr].InsertBefore(vl, vl1);
      edges_[vl1].InsertAfter(vl, vr);
      vl = vl1;
    } else {
      edges_[vl].InsertAfter(vr, vr1);
      edges_[vr1].InsertBefore(vr, vl);
      vr = vr1;
    }

  } while (true);
}

void Mesh::LowerCommonTangent(int middle, int lct[]) {
  bool moved;
  int to_test;

  lct[0] = middle;
  lct[1] = middle + 1;

  do {
    moved = false;

    to_test = edges_[lct[0]].last()->data();
    while (robust_predicates::orient2d(vertices_[lct[0]], vertices_[lct[1]], vertices_[to_test]) < 0) {
      moved = true;
      lct[0] = to_test;
      to_test = edges_[lct[0]].last()->data();
    }

    to_test = edges_[lct[1]].first()->data();
    while (robust_predicates::orient2d(vertices_[lct[0]], vertices_[lct[1]], vertices_[to_test]) < 0) {
      moved = true;
      lct[1] = to_test;
      to_test = edges_[lct[1]].first()->data();
    }
  } while (moved);
}

void Mesh::BuildEdge(int p1, int p2) {
  edges_[p1].Prepend(p2);
  edges_[p2].Append(p1);
}

void Mesh::DestroyEdge(int p1, int p2) {
  edges_[p1].Remove(p2);
  edges_[p2].Remove(p1);
}

void Mesh::ReadFile(const char* filename) {
  size_ = 0;
  ifstream points_file (filename);

  if (!points_file.is_open()) {
    cout << "Unable to open file " << filename << endl;
    exit(69);
  }

  int i;
  double x, y;
  double dx, dy;
  string line;

  // Count lines in the file
  while (getline(points_file, line)) {
    size_++;
  }

  points_file.clear();
  points_file.seekg(0, ios::beg);

  // Initializing edges and vertices arrays
  edges_ = new DoublyLinkedList<int>[size_];
  vertices_ = new double*[size_];
  vertices_[0] = new double[2 * size_];
  for (i=1; i<size_; i++) {
    vertices_[i] = &vertices_[0][i * 2];
  }

  // Read and store each vertex
  points_file >> x >> y;
  vertices_[0][0] = x;
  vertices_[0][1] = y;
  x_min_ = x_max_= x;
  y_min_ = y_max_= y;

  for (i=1; i<size_; i++) {
    points_file >> x >> y;
    vertices_[i][0] = x;
    vertices_[i][1] = y;
    x_min_ = MIN(x_min_, x);
    x_max_ = MAX(x_max_, x);
    y_min_ = MIN(y_min_, y);
    y_max_ = MAX(y_max_, y);
  }

  // Define a frame around the vertices cloud
  dx = (x_max_ - x_min_) * 0.1;
  x_min_ = x_min_ - dx;
  x_max_ = x_max_ + dx;
  dy = (y_max_ - y_min_) * 0.1;
  y_min_ = y_min_ - dy;
  y_max_ = y_max_ + dy;

  points_file.close();
}

void Mesh::Shake() {
  double delta_max = MAX((x_max_ - x_min_), (y_max_ - y_min_)) * 1e-8;
  for (int i=0; i<size_; i++) {
    vertices_[i][0] += (((double) std::rand()) / RAND_MAX) * delta_max;
    vertices_[i][1] += (((double) std::rand()) / RAND_MAX) * delta_max;
  }
}

void Mesh::CountEdges() {
  int i;
  n_edges_ = 0;
  for (i=0; i<size_; i++) {
    n_edges_ += edges_[i].Length();
  }
  n_edges_ /= 2;
}

void Mesh::Vertices(double* vertices) {
  for (int i=0; i<size_; i++) {
    vertices[3 * i] = vertices_[i][0];
    vertices[3 * i + 1] = vertices_[i][1];
    vertices[3 * i + 2] = 0.f;
  }
}

void Mesh::Edges(uint* indices) {
  int v1, i;
  n_edges_ = 0;
  DoublyLinkedListElem<int> *edge;
  DoublyLinkedListElem<int> *first;

  i = 0;
  for (v1=0; v1<size_; v1++) {
    edge = first = edges_[v1].first();
    if (edge == nullptr)
      continue;
    do {
      if (edge->data() > v1) {
        indices[2 * i] = v1;
        indices[2 * i + 1] = edge->data();
        i++;
      }
      edge = edge->next();
    } while (edge != first);
  }
}

void Mesh::SortVertices() {
  double *vertices_start = vertices_[0];
  sort(vertices_, vertices_ + size_, [](const double* v1, const double* v2) {return (v1[0] < v2[0]);});
  double *temp = new double[2 * size_];
  for (int i=0; i<size_; i++) {
    temp[2 * i] = vertices_[i][0];
    temp[2 * i + 1] = vertices_[i][1];
    vertices_[i] = temp + 2 * i;
  }
  delete[] vertices_start;
}

void Mesh::PrintEdges() {
  for (int i=0; i<size_; i++) {
    cout << i << " : ";
    edges_[i].Print();
  }
}
