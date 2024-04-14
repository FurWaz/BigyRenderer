#pragma once
#include "Light.hpp"

class PointLight : public Light
{
protected:
    float range;

public:
    PointLight(Vec3f pos, Quaternion rot, Color col, float intensity, float range)
        : Light(pos, rot, col, intensity), range(range)
    {

    }

    float getIntensity(Vec3f point, Vec3f normal) const
    {
        Vec3f shift = position - point;
        Vec3f lightDir = shift.normalize();
        float distance = shift.length();
        float angle = lightDir.dot(normal);
        if (angle < 0.f) angle = 0.f;
        float range = 1.f - (distance / this->range);
        if (range < 0.f) range = 0.f;
        if (range > 1.f) range = 1.f;
        return angle * intensity;
    }
};
