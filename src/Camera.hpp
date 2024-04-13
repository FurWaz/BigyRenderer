#pragma once
#include "Object.hpp"

class Camera: public Object
{
public:
    Camera();
    Camera(const Vec3f &position, const Quaternion &rotation);
    Camera(const Camera &other);
    ~Camera();

    void lookAt(Vec3f target);
};
