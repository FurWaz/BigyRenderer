#pragma once
#include "Light.hpp"
#include "Model.hpp"

class PointLight : public Light
{
protected:
    float range;

public:
    PointLight(Vec3f pos, Quaternion rot, Color col, float intensity, float range)
        : Light(pos, rot, col, intensity), range(range)
    {

    }

    Vec2i projectPoint(const Vec3f& point) const
    {
        return Renderer::ProjectPoint(shadowMapSize, shadowMapSize, point); // TODO : change for 360deg view
    }

    float getIntensity(const Vec3f& point, const Vec3f& normal) const
    {
        Vec3f shift = position - point;
        Vec3f lightDir = shift.normalize();
        float distance = shift.length();
        float angle = lightDir.dot(normal);
        if (angle < 0.f) angle = 0.f;
        float range = 1.f - (distance / this->range);
        if (range < 0.f) range = 0.f;
        if (range > 1.f) range = 1.f;
        return angle * intensity * range;
    }
};
