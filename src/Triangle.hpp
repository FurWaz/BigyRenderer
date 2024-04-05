#pragma once
#include "Vec3.hpp"

class Triangle
{    
public:
    size_t ver_i[3];
    size_t tex_i[3];
    size_t nor_i[3];
    
    Triangle();
    Triangle(size_t v0, size_t v1, size_t v2, size_t t0, size_t t1, size_t t2, size_t n0, size_t n1, size_t n2);
    Triangle(const Triangle &other);
    ~Triangle();

    friend std::ostream& operator<<(std::ostream& os, const Triangle& tri)
    {
        os << "  Vertices : " << tri.ver_i[0] << " " << tri.ver_i[1] << " " << tri.ver_i[2] << std::endl;
        os << "  Textures : " << tri.tex_i[0] << " " << tri.tex_i[1] << " " << tri.tex_i[2] << std::endl;
        os << "  Normals : " << tri.nor_i[0] << " " << tri.nor_i[1] << " " << tri.nor_i[2] << std::endl;
        return os;
    }
};
