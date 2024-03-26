#include "Point.hpp"

Point::Point()
    : position(Vec3f()), normal(Vec3f(0, 1, 0))
{

}

Point::Point(Vec3f position, Vec3f normal)
    : position(position), normal(normal)
{

}

Point::Point(const Point &other)
    : position(other.position), normal(other.normal)
{

}

Point::~Point()
{

}
