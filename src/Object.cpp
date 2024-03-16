#include "Object.hpp"

Object::Object() : position(), rotation() {}

Object::Object(const Vec3f &position, const Quaternion &rotation)
    : position(position), rotation(rotation) {}

Object::Object(const Object &other)
    : position(other.position), rotation(other.rotation) {}

Object::~Object() {}
