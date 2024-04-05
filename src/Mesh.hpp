#pragma once
#include <string>
#include "Object.hpp"
#include "Triangle.hpp"
#include "Material.hpp"
#include "Vec2.hpp"

class Mesh: public Object
{
public:
    Vec3f* vertices;
    size_t n_vertices;
    Vec2f* textures;
    size_t n_textures;
    Vec3f* normals;
    size_t n_normals;

    Triangle* triangles;
    size_t n_triangles;

    Material material;

    Mesh();
    Mesh(Vec3f* vertices, size_t n_vertices, Vec2f* textures, size_t n_textures, Vec3f* normals, size_t n_normals, Triangle* triangles, size_t n_triangles);
    Mesh(const Mesh &other);
    ~Mesh();

    bool valid();
};
