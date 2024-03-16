#pragma once
#include "Vec3.hpp"

class Triangle
{    
public:
    size_t vertices_index[3];
    const Vec3f* vertices;
    
    Triangle();
    Triangle(const Vec3f* vertices, size_t v0, size_t v1, size_t v2);
    Triangle(const Triangle &other);
    ~Triangle();
};
