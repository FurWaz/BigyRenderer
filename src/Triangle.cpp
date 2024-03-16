#include "Triangle.hpp"

Triangle::Triangle()
    : vertices(new Vec3f[3]), vertices_index{0, 1, 2} {}

Triangle::Triangle(const Vec3f* vertices, size_t v0, size_t v1, size_t v2)
    : vertices(vertices), vertices_index{v0, v1, v2} {}

Triangle::Triangle(const Triangle &other)
    : vertices(other.vertices),
    vertices_index{other.vertices_index[0], other.vertices_index[1], other.vertices_index[2]}
{
    
}

Triangle::~Triangle() {}
