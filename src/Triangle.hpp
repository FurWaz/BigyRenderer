#pragma once
#include "Vec3.hpp"

class Triangle
{    
public:
    size_t vert_i[3];
    
    Triangle();
    Triangle(size_t v0, size_t v1, size_t v2);
    Triangle(const Triangle &other);
    ~Triangle();
};
