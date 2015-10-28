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
    double **vertices_;
    // Length : size_
    DoublyLinkedList<int> *edges_;

    // Defines an outer rectangle
    double x_min_;
    double x_max_;
    double y_min_;
    double y_max_;

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
    void DestroyEdge(int, int);

    void StoreEdgesArray();
    void LowerCommonTangent(int, int, int[]);
    int RightCandidate(int, int);
    int LeftCandidate(int, int);

  public:

    // Default Constructor
    Mesh() : size_(0), n_edges_(0), vertices_(0), edges_(0) {}

    // Destructor
    ~Mesh();

    // Getters
    int size() const { return size_; }
    double n_edges() const { return n_edges_; }
    double x_max() const { return x_max_; }
    double x_min() const { return x_min_; }
    double y_max() const { return y_max_; }
    double y_min() const { return y_min_; }
    void Vertices(double*);
    void Edges(uint*);

    // Generate Delaunay Triangulation
    void Triangulate(const char*);
};

#endif // MESH_MESH_H_
