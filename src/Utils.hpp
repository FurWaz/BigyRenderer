#pragma once
#include "Vec2.hpp"
#include "Vec3.hpp"

namespace Utils
{
    Vec3f barycentric(Vec2i p1, Vec2i p2, Vec2i p3, Vec2i p)
    {
        Vec3i u = Vec3i(p3.x - p1.x, p2.x - p1.x, p1.x - p.x);
        Vec3i v = Vec3i(p3.y - p1.y, p2.y - p1.y, p1.y - p.y);
        Vec3i w = u.cross(v);

        if (std::abs(w.z) < 1) return Vec3f(-1, 1, 1);

        return Vec3f(1.0f - (w.x + w.y) / (float)w.z, w.y / (float)w.z, w.x / (float)w.z);
    }
};
