#include "Object.hpp"

Object::Object() : position(), rotation() {}

Object::Object(const Vec3f &position, const Quaternion &rotation)
    : position(position), rotation(rotation) {}

Object::Object(const Object &other)
    : position(other.position), rotation(other.rotation) {}

Object::~Object() {}

Vec3f Object::forward() const
{
    // NOTE : not returning rotation * Vec3f(0, 0, 1)
    // because the camera rotation's conjugate should be applied to the object
    return Vec3f(0, 0, 1);
}
