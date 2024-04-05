#include "Triangle.hpp"

Triangle::Triangle()
    : ver_i{0, 0, 0}, tex_i{0, 0, 0}, nor_i{0, 0, 0} {}

Triangle::Triangle(size_t v0, size_t v1, size_t v2, size_t t0, size_t t1, size_t t2, size_t n0, size_t n1, size_t n2)
    : ver_i{v0, v1, v2}, tex_i{t0, t1, t2}, nor_i{n0, n1, n2} {}

Triangle::Triangle(const Triangle &other)
    : ver_i{other.ver_i[0], other.ver_i[1], other.ver_i[2]},
      tex_i{other.tex_i[0], other.tex_i[1], other.tex_i[2]},
      nor_i{other.nor_i[0], other.nor_i[1], other.nor_i[2]}
{
    
}

Triangle::~Triangle() {}
