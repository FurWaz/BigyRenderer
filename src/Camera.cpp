#include "Camera.hpp"

Camera::Camera() : Object() {}

Camera::Camera(const Vec3f &position, const Quaternion &rotation)
    : Object(position, rotation) {}

Camera::Camera(const Camera &other)
    : Object(other.position, other.rotation) {}

Camera::~Camera() {}

void Camera::lookAt(Vec3f target)
{
    Vec3f lookVector = (target - position).normalize();
    Quaternion rotY = Quaternion::Euler(
        0,
        atan2(-lookVector.z, lookVector.x) - 1.57f,
        0
    );
    Quaternion rotX = Quaternion::Euler(
        atan2(lookVector.y, sqrt(lookVector.x * lookVector.x + lookVector.z * lookVector.z)),
        0,
        0
    );
    this->rotation = rotY * rotX;
}
