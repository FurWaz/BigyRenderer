#include "Object.hpp"

Object::Object() : position(), rotation() {}

Object::Object(const Vec3f &position, const Quaternion &rotation)
    : position(position), rotation(rotation) {}

Object::Object(const Object &other)
    : position(other.position), rotation(other.rotation) {}

Object::~Object() {}

Vec3f Object::forward() const
{
    return Vec3f(0, 0, 1);
}

Vec3f Object::right() const
{
    return Vec3f(1, 0, 0);
}