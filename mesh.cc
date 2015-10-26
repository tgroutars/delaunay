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
    float area = TriangleArea(vertices_[start], vertices_[middle], vertices_[end]);
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

void Mesh::MergeMeshes(int start, int middle, int end) {
  int lower_common_tangent[2], upper_common_tangent[2];

  LowerCommonTangent(middle, middle + 1, lower_common_tangent);
  UpperCommonTangent(middle, middle + 1, upper_common_tangent);
  return;
}

void Mesh::LowerCommonTangent(int left, int right, int lct[]) {
  bool moved;
  int to_test;

  lct[0] = left;
  lct[1] = right;

  do {
    moved = false;

    to_test = edges_[lct[0]].last()->data();
    while (TriangleArea(vertices_[lct[0]], vertices_[lct[1]], vertices_[to_test]) < 0) {
      moved = true;
      lct[0] = to_test;
      to_test = edges_[lct[0]].last()->data();
    }

    to_test = edges_[lct[1]].first()->data();
    while (TriangleArea(vertices_[lct[0]], vertices_[lct[1]], vertices_[to_test]) < 0) {
      moved = true;
      lct[1] = to_test;
      to_test = edges_[lct[1]].first()->data();
    }
  } while (moved);
}

void Mesh::UpperCommonTangent(int left, int right, int uct[2]) {
  bool moved;
  int to_test;

  uct[0] = left;
  uct[1] = right;

  do {
    moved = false;

    to_test = edges_[uct[0]].first()->data();
    while (TriangleArea(vertices_[uct[0]], vertices_[uct[1]], vertices_[to_test]) > 0) {
      moved = true;
      uct[0] = to_test;
      to_test = edges_[uct[0]].first()->data();
    }

    to_test = edges_[uct[1]].last()->data();
    while (TriangleArea(vertices_[uct[0]], vertices_[uct[1]], vertices_[to_test]) > 0) {
      moved = true;
      uct[1] = to_test;
      to_test = edges_[uct[1]].last()->data();
    }
  } while (moved);
}

void Mesh::BuildEdge(int p1, int p2) {
  edges_[p1].Prepend(p2);
  edges_[p2].Append(p1);
}

void Mesh::ReadFile(const char* filename) {
  size_ = 0;
  ifstream points_file (filename);

  if (!points_file.is_open()) {
    cout << "Unable to open file " << filename << endl;
    exit(69);
  }

  int i;
  float x, y;
  float dx, dy;
  string line;

  while (getline(points_file, line)) {
    size_++;
  }

  points_file.clear();
  points_file.seekg(0, ios::beg);

  edges_ = new DoublyLinkedList<int>[size_];
  vertices_ = new float*[size_];
  vertices_[0] = new float[2 * size_];
  for (i=1; i<size_; i++) {
    vertices_[i] = &vertices_[0][i * 2];
  }

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

  dx = (x_max_ - x_min_) * 0.1;
  x_min_ = x_min_ - dx;
  x_max_ = x_max_ + dx;
  dy = (y_max_ - y_min_) * 0.1;
  y_min_ = y_min_ - dy;
  y_max_ = y_max_ + dy;

  points_file.close();
}

void Mesh::Shake() {
  int i;
  float dx_max = (x_max_ - x_min_) * 1e-8,
        dy_max = (y_max_ - y_min_) * 1e-8;

  for (i=0; i<size_; i++) {
    vertices_[i][0] += (((float) std::rand()) / RAND_MAX) * dx_max;
    vertices_[i][1] += (((float) std::rand()) / RAND_MAX) * dy_max;
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

void Mesh::Vertices(float* vertices) {
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
  sort(&vertices_[0], &vertices_[size_ - 1], [](const float* v1, const float* v2) {return v1[0] < v2[0];});
}
