#pragma once
#include "Vec3.hpp"

class Point
{
public:
    Vec3f position;
    Vec3f normal;

    Point();
    Point(Vec3f position, Vec3f normal);
    Point(const Point &other);
    ~Point();
};

