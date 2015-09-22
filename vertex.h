#ifndef MESH_VERTEX_H_
#define MESH_VERTEX_H_

#include "linked_list.h"

using namespace std;

class Vertex;

class Vertex {
    float position_[2];
    float height_;
    LinkedList<Vertex> *neighbors_;
  public:
    Vertex () : neighbors_(nullptr) {}
    Vertex (float, float, float);
    ~Vertex ();
    void print ();
};

#endif // MESH_VERTEX_H_
