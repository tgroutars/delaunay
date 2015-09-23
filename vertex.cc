#include <iostream>

#include "vertex.h"


Vertex::Vertex(float x, float y, float z) {
  position_[0] = x;
  position_[1] = y;
  height_ = z;
  neighbors_ = nullptr;
}

Vertex::~Vertex() {}

std::ostream& operator<<(std::ostream& os, const Vertex vertex) {
  return os << "Point" << endl
            << "x : "  << vertex.position_[0] << endl
            << "y : "  << vertex.position_[1] << endl
            << "z : "  << vertex.height_ << endl;
}
