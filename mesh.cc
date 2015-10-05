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
}

void Mesh::Triangulate(const char* filename) {
  ReadFile(filename);
  Shake();
  DelaunayDC(0, size_);

}

void Mesh::DelaunayDC(int start, int end) {
  int size = end - start;
  if (size < 2)
    return;
  if (size == 2)
    BuildEdge(start, end - 1);
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
