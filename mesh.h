#ifndef MESH_MESH_H_
#define MESH_MESH_H_

#include "vertex.h"

class Mesh {

    // Number of Vertices
    int size_;

    // Array of Vertices in the mesh
    Vertex *vertices_;

    // Defines an outer rectangle
    float x_min_;
    float x_max_;
    float y_min_;
    float y_max_;

    // Initialize array of vertices from file
    void read_file(const char*);

    // Move points randomly to avoid colinear points
    void shake();

  public:

    // Default Constructor
    Mesh() : size_(0), vertices_(nullptr) {}

    // Destructor
    ~Mesh();

    // Generate Delaunay Triangulation
    void triangulate(const char*);
};

#endif // MESH_MESH_H_
