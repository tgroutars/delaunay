#ifndef MESH_MESH_H_
#define MESH_MESH_H_

#include "vertex.h"

class Mesh {
    int size_;
    Vertex *vertices_;
  public:
    Mesh() : size_(0), vertices_(nullptr) {}
    ~Mesh();
    void read_file(const char*);
};

#endif // MESH_MESH_H_
