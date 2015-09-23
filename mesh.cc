#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>

#include "mesh.h"

#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MAX(a,b) ((a) > (b) ? (a) : (b))


Mesh::~Mesh() {
  if (vertices_ != nullptr) {
    delete[] vertices_;
  }
}

void Mesh::triangulate(const char* filename) {
  read_file(filename);
  for (int i=0; i<size_; i++) {
    cout << vertices_[i] << endl;
  }
  cout << x_min_ << endl;
  cout << x_max_ << endl;
  cout << y_min_ << endl;
  cout << y_max_ << endl;
  shake();
}

void Mesh::read_file(const char* filename) {
  size_ = 0;
  ifstream points_file (filename);

  if (points_file.is_open()) {
    int i;
    double x, y, z;
    float dx, dy;
    string line;

    while (getline(points_file, line)) {
      size_++;
    }

    points_file.clear();
    points_file.seekg(0, ios::beg);

    vertices_ = new Vertex[size_];
    points_file >> x >> y >> z;
    vertices_[0] = Vertex(x, y, z);
    x_min_ = x_max_= x;
    y_min_ = y_max_= y;

    for (i=1; i<size_; i++) {
      points_file >> x >> y >> z;
      vertices_[i] = Vertex(x, y, z);
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

void Mesh::shake() {
  int i;
  float dx_max = (x_max_ - x_min_) * 1e-8,
        dy_max = (y_max_ - y_min_) * 1e-8;

  for (i=0; i<size_; i++) {
    vertices_[i].position_[0] += (((float) std::rand()) / RAND_MAX) * dx_max;
    vertices_[i].position_[1] += (((float) std::rand()) / RAND_MAX) * dy_max;
  }
}
