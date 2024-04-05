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
{
    n_vertices = other.n_vertices;
    n_triangles = other.n_triangles;

    vertices = new Vec3f[n_vertices];
    for (size_t i = 0; i < n_vertices; i++)
        vertices[i] = other.vertices[i];

    textures = new Vec2f[n_textures];
    for (size_t i = 0; i < n_textures; i++)
        textures[i] = other.textures[i];

    normals = new Vec3f[n_normals];
    for (size_t i = 0; i < n_normals; i++)
        normals[i] = other.normals[i];

    triangles = new Triangle[n_triangles];
    for (size_t i = 0; i < n_triangles; i++)
        triangles[i] = other.triangles[i];

    material = other.material;
}

Mesh::~Mesh()
{
    delete[] vertices;
    delete[] textures;
    delete[] normals;
    delete[] triangles;
}

bool Mesh::valid()
{
    return vertices != nullptr && triangles != nullptr;
}
