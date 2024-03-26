#pragma once
#include <string>
#include "Object.hpp"
#include "Triangle.hpp"
#include "Point.hpp"

class Model: public Object
{
public:
    Point* vertices;
    size_t n_vertices;

    Triangle* triangles;
    size_t n_triangles;

    Model();
    Model(Point* vertices, size_t n_vertices, Triangle* triangles, size_t n_triangles);
    Model(const Model &other);
    ~Model();

    bool valid();
};
