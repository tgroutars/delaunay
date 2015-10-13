#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>

#include "mesh.h"

#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MAX(a,b) ((a) > (b) ? (a) : (b))

using namespace std;

Mesh::~Mesh() {
  if (vertices_ != nullptr) {
    delete[] vertices_;
  }
  if (edges_ != nullptr) {
    delete[] edges_;
  }
  if (edges_array_ != nullptr) {
    delete[] edges_array_;
  }
}

void Mesh::Triangulate(const char* filename) {
  ReadFile(filename);
  Shake();
  DelaunayDC(0, size_ - 1);
  StoreEdgesArray();
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
  int middle = (start + end) / 2.;
  if (size == 3) {
    BuildEdge(start, middle);
    BuildEdge(middle, end);
    BuildEdge(end, start);
    return;
  }
  DelaunayDC(start, middle);
  DelaunayDC(middle + 1, end);
  MergeMeshes(start, middle, end);
}

void Mesh::MergeMeshes(int start, int middle, int end) {
  return;
}

void Mesh::BuildEdge(int p1, int p2) {
  edges_[p1].Prepend(p2);
  edges_[p2].Append(p1);
}

void Mesh::ReadFile(const char* filename) {
  size_ = 0;
  ifstream points_file (filename);

  if (points_file.is_open()) {
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
    vertices_ = new float[size_ * 2];

    points_file >> x >> y;
    vertices_[0] = x;
    vertices_[1] = y;
    x_min_ = x_max_= x;
    y_min_ = y_max_= y;

    for (i=1; i<size_; i++) {
      points_file >> x >> y;
      vertices_[2 * i] = x;
      vertices_[2 * i + 1] = y;
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

  } else {
    cout << "Unable to open file";
  }

}

void Mesh::Shake() {
  int i;
  float dx_max = (x_max_ - x_min_) * 1e-8,
        dy_max = (y_max_ - y_min_) * 1e-8;

  for (i=0; i<size_; i++) {
    vertices_[2 * i] += (((float) std::rand()) / RAND_MAX) * dx_max;
    vertices_[2 * i + 1] += (((float) std::rand()) / RAND_MAX) * dy_max;
  }
}

void Mesh::StoreEdgesArray() {

  int v1, i = 0;
  n_edges_ = 0;
  DoublyLinkedListElem<int> *edge;
  DoublyLinkedListElem<int> *first;

  for (v1=0; v1<size_; v1++) {
    n_edges_ += edges_[i].Length();
  }
  n_edges_ /= 2;

  edges_array_ = new int[n_edges_ * 2];
  for (v1=0; v1<size_; v1++) {
    edge = first = edges_[v1].first();
    if (edge == nullptr)
      continue;
    do {
      if (edge->data() > v1) {
        edges_array_[2 * i] = v1;
        edges_array_[2 * i + 1] = edge->data();
        i++;
      }
      edge = edge->next();
    } while (edge != first);
  }
}
