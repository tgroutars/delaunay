#include <iostream>
#include <fstream>
#include <string>

#include "mesh.h"


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
}

void Mesh::read_file(const char* filename) {
  size_ = 0;
  ifstream points_file (filename);

  if (points_file.is_open()) {
    int i;
    double x, y, z;
    string line;

    while (getline(points_file, line)) {
      size_++;
    }

    points_file.clear();
    points_file.seekg(0, ios::beg);

    vertices_ = new Vertex[size_];
    i = 0;
    for (i=0; i<size_; i++) {
      points_file >> x >> y >> z;
      vertices_[i] = Vertex(x, y, z);
    }
    points_file.close();

  } else {
    cout << "Unable to open file";
  }

}
