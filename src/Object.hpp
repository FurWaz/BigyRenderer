#pragma once
#include "Vec3.hpp"
#include "Quaternion.hpp"

#define PI 3.141592f
#define SEMI_PI 1.570796f

class Object
{
public:
    Vec3f position;
    Quaternion rotation;

    Vec3f forward() const;
    Vec3f right() const;

    Object();
    Object(Vec3f position, Quaternion rotation);
    Object(const Object &other);
    ~Object();
};
