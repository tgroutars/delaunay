#ifndef MESH_VERTEX_H_
#define MESH_VERTEX_H_

#include "linked_list.h"

class Vertex;

class Vertex {
    double position_[2];
    LinkedList<Vertex> neighbors_;
};

#endif // MESH_VERTEX_H_
