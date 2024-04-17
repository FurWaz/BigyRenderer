#pragma once
#include "Light.hpp"
#include "Model.hpp"
#include "Renderer.hpp"
#include "math.h"

class PointLight : public Light
{
protected:
    float range;

public:
    PointLight(Vec3f pos, Quaternion rot, Color col, float intensity, float range)
        : Light(pos, rot, col, intensity), range(range)
    {

    }

    ~PointLight()
    {
        
    }

    Vec2i projectPoint(const Vec3f& point) const
    {
        Vec3f local = rotation.conjugate() * (point - position);
        Vec2f screen(
            atan2(local.x, -local.z) / PI, // between -1 and 1
            atan2(local.y, sqrt(local.x * local.x + local.z * local.z)) / PI // between -1 and 1
        );
        Vec2i vec(
            (int) ((screen.x * 0.5f + 0.5f) * shadowMapSize),
            (int) ((screen.y * 0.5f + 0.5f) * shadowMapSize)
        );
        return vec;
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
