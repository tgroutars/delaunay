#ifndef MESH_MESH_H_
#define MESH_MESH_H_

#include "linked_list.h"

class Mesh {

    // Number of Vertices
    int size_;
    // Number of edges
    int n_edges_;

    // Arrays of vertices and edges in the mesh
    // Length : size_ x 2
    float **vertices_;
    // Length : size_
    DoublyLinkedList<int> *edges_;

    // Defines an outer rectangle
    float x_min_;
    float x_max_;
    float y_min_;
    float y_max_;

    // Initialize array of vertices from file
    void ReadFile(const char*);

    // Move points randomly to avoid colinear points
    void Shake();

    // Sort points from left to right
    void SortVertices();

    // Build the mesh
    void CountEdges();
    void DelaunayDC(int, int);
    void MergeMeshes(int, int, int);
    void BuildEdge(int, int);
    void StoreEdgesArray();
    void LowerCommonTangent(int, int, int[]);
    void UpperCommonTangent(int, int, int[]);

  public:

    // Default Constructor
    Mesh() : size_(0), n_edges_(0), vertices_(0), edges_(0) {}

    // Destructor
    ~Mesh();

    // Getters
    int size() const { return size_; }
    float n_edges() const { return n_edges_; }
    float x_max() const { return x_max_; }
    float x_min() const { return x_min_; }
    float y_max() const { return y_max_; }
    float y_min() const { return y_min_; }
    void Vertices(float*);
    void Edges(uint*);

    // Generate Delaunay Triangulation
    void Triangulate(const char*);
};

#endif // MESH_MESH_H_
