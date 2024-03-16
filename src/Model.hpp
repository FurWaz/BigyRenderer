#pragma once
#include <string>
#include "Triangle.hpp"
#include "Vec3.hpp"

class Model
{
public:
    Vec3f* vertices;
    size_t n_vertices;

    Triangle* triangles;
    size_t n_triangles;

    Model();
    Model(Vec3f* vertices, size_t n_vertices, Triangle* triangles, size_t n_triangles);
    Model(const Model &other);
    ~Model();

    bool valid();
};
