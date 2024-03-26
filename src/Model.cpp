#include "Model.hpp"

Model::Model(): vertices(nullptr), n_vertices(0) {}

Model::Model(Point* vertices, size_t n_vertices, Triangle* triangles, size_t n_triangles)
    : vertices(vertices), n_vertices(n_vertices), triangles(triangles), n_triangles(n_triangles)
{
    
}

Model::Model(const Model &other)
{
    n_vertices = other.n_vertices;
    n_triangles = other.n_triangles;

    vertices = new Point[n_vertices];
    for (size_t i = 0; i < n_vertices; i++)
        vertices[i] = other.vertices[i];

    triangles = new Triangle[n_triangles];
    for (size_t i = 0; i < n_triangles; i++)
        triangles[i] = other.triangles[i];
}

Model::~Model()
{
    delete[] vertices;
}

bool Model::valid()
{
    return vertices != nullptr && triangles != nullptr;
}
