#include "Object.hpp"

Object::Object() : position(), rotation(), scale() {}

Object::Object(Vec3f position, Quaternion rotation, Vec3f scale)
    : position(position), rotation(rotation), scale(scale) {}

Object::Object(const Object &other)
    : position(other.position), rotation(other.rotation), scale(other.scale) {}

Object::~Object() {}

Vec3f Object::forward() const
{
    return rotation * Vec3f(0, 0, -1);
}

Vec3f Object::right() const
{
    return rotation * Vec3f(1, 0, 0);
}