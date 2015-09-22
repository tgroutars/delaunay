#include <iostream>

#include "vertex.h"


Vertex::Vertex(float x, float y, float z) {
  position_[0] = x;
  position_[1] = y;
  height_ = z;
  neighbors_ = nullptr;
}

Vertex::~Vertex() {}

void Vertex::print() {
  cout << "\nPoint" << endl;
  cout << "x : " << position_[0] << endl;
  cout << "y : " << position_[1] << endl;
}
