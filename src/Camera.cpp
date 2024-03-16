#include "Camera.hpp"

Camera::Camera() : Object() {}

Camera::Camera(const Vec3f &position, const Quaternion &rotation)
    : Object(position, rotation) {}

Camera::Camera(const Camera &other)
    : Object(other.position, other.rotation) {}

Camera::~Camera() {}
