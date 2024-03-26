#include "Triangle.hpp"

Triangle::Triangle()
    : vert_i{0, 1, 2} {}

Triangle::Triangle(size_t v0, size_t v1, size_t v2)
    : vert_i{v0, v1, v2} {}

Triangle::Triangle(const Triangle &other)
    : vert_i{other.vert_i[0], other.vert_i[1], other.vert_i[2]}
{
    
}

Triangle::~Triangle() {}
