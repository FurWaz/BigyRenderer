#include "Mesh.hpp"

Mesh::Mesh():
    vertices(nullptr), n_vertices(0),
    textures(nullptr), n_textures(0),
    normals(nullptr), n_normals(0),
    triangles(nullptr), n_triangles(0)
{
    
}

Mesh::Mesh(Vec3f* vertices, size_t n_vertices, Vec2f* textures, size_t n_textures, Vec3f* normals, size_t n_normals, Triangle* triangles, size_t n_triangles)
    : vertices(vertices), n_vertices(n_vertices),
      textures(textures), n_textures(n_textures),
      normals(normals),   n_normals(n_normals),
      triangles(triangles), n_triangles(n_triangles)
{
    
}

Mesh::Mesh(const Mesh &other)
    : vertices(new Vec3f[other.n_vertices]), n_vertices(other.n_vertices),
      textures(new Vec2f[other.n_textures]), n_textures(other.n_textures),
      normals(new Vec3f[other.n_normals]),   n_normals(other.n_normals),
      triangles(new Triangle[other.n_triangles]), n_triangles(other.n_triangles)
{
    for (size_t i = 0; i < n_vertices; i++)
        this->vertices[i] = Vec3f(other.vertices[i]);
    for (size_t i = 0; i < n_textures; i++)
        this->textures[i] = Vec2f(other.textures[i]);
    for (size_t i = 0; i < n_normals; i++)
        this->normals[i] = Vec3f(other.normals[i]);
    for (size_t i = 0; i < n_triangles; i++)
        this->triangles[i] = Triangle(other.triangles[i]);
}

Mesh::~Mesh()
{
    delete[] vertices;
    delete[] textures;
    delete[] normals;
    delete[] triangles;
}

const bool Mesh::valid() const
{
    return n_vertices > 0 && n_textures  > 0 &&
           n_normals  > 0 && n_triangles > 0;
}
