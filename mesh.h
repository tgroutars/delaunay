#ifndef MESH_MESH_H_
#define MESH_MESH_H_

#include "linked_list.h"

class Mesh {

    // Number of Vertices
    int size_;
    int n_edges_;

    // Arrays of vertices and edges in the mesh
    float *vertices_;
    DoublyLinkedList<int> *edges_;
    int *edges_array_;

    // Defines an outer rectangle
    float x_min_;
    float x_max_;
    float y_min_;
    float y_max_;

    // Initialize array of vertices from file
    void ReadFile(const char*);

    // Move points randomly to avoid colinear points
    void Shake();

    // Build the mesh
    void DelaunayDC(int, int);
    void BuildEdge(int, int);
    void StoreEdgesArray();

  public:

    // Default Constructor
    Mesh() : size_(0), n_edges_(0), vertices_(0), edges_(0), edges_array_(0) {}

    // Destructor
    ~Mesh();

    // Getters
    int size() const { return size_; }
    float n_edges() const { return n_edges_; }
    float x_max() const { return x_max_; }
    float x_min() const { return x_min_; }
    float y_max() const { return y_max_; }
    float y_min() const { return y_min_; }
    const float* vertices() const { return vertices_; }
    const int* edges() const { return edges_array_; }

    // Generate Delaunay Triangulation
    void Triangulate(const char*);
};

#endif // MESH_MESH_H_
