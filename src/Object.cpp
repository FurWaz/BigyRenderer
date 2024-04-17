#include "Object.hpp"

Object::Object() : position(), rotation() {}

Object::Object(Vec3f position, Quaternion rotation)
    : position(position), rotation(rotation) {}

Object::Object(const Object &other)
    : position(other.position), rotation(other.rotation) {}

Object::~Object() {}

Vec3f Object::forward() const
{
    return rotation * Vec3f(0, 0, -1);
}

Vec3f Object::right() const
{
    return rotation * Vec3f(1, 0, 0);
}