#ifndef MESH_MESH_H_
#define MESH_MESH_H_

#include "vertex.h"

class Mesh {

    // Number of Vertices
    int size_;

    // Array of Vertices in the mesh
    Vertex *vertices_;

    // Initialize array of vertices from file
    void read_file(const char*);

  public:

    // Default Constructor
    Mesh() : size_(0), vertices_(nullptr) {}

    // Destructor
    ~Mesh();

    // Generate Delaunay Triangulation
    void triangulate(const char*);
};

#endif // MESH_MESH_H_
