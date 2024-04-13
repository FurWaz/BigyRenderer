#pragma once
#include "Light.hpp"

class PointLight : public Light
{
private:
    float clamp(float val, float min, float max) const
    {
        return val > max ? max : val < min ? min : val;
    }

    float max(float v1, float v2) const
    {
        return v1 > v2 ? v1 : v2;
    }


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
        float angle = max(lightDir.dot(normal), 0.0f);
        float range = clamp(1.f - (distance / this->range), 0.0f, 1.0f);
        return angle * intensity;
    }
};
