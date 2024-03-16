#pragma once
#include "Vec3.hpp"
#include "Quaternion.hpp"

class Object
{
public:
    Vec3f position;
    Quaternion rotation;

    Object();
    Object(const Vec3f &position, const Quaternion &rotation);
    Object(const Object &other);
    ~Object();
};
