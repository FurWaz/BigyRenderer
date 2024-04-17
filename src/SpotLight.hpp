#pragma once
#include "Light.hpp"
#include "Model.hpp"
#include "Renderer.hpp"

class SpotLight : public Light
{
protected:
    float range;
    float cone;

public:
    SpotLight(Vec3f pos, Quaternion rot, Color col, float intensity, float range, float cone)
        : Light(pos, rot, col, intensity), range(range), cone(cone)
    {

    }

    ~SpotLight()
    {

    }

    Vec2i projectPoint(const Vec3f& point) const
    {
        Vec3f local = rotation.conjugate() * (point - position);
        Vec2f screen(
            atan2(local.x, -local.z) / PI, // between -1 and 1
            atan2(local.y, sqrt(local.x * local.x + local.z * local.z)) / PI // between -1 and 1
        );
        // screen *= 2.f;
        screen *= PI / cone;
        Vec2i vec(
            (int) ((screen.x * 0.5f + 0.5f) * shadowMapSize),
            (int) ((screen.y * 0.5f + 0.5f) * shadowMapSize)
        );
        return vec;
    }

    float getIntensity(const Vec3f& point, const Vec3f& normal) const
    {
        Vec3f shift = position - point; // from point towards the light
        Vec3f lightDir = shift.normalize();
        float distance = shift.length();
        float angle = lightDir.dot(normal);
        if (angle < 0.f) angle = 0.f;
        float range = 1.f - (distance / this->range);
        if (range < 0.f) range = 0.f;
        if (range > 1.f) range = 1.f;
        float projX = lightDir.dot(rotation.conjugate() * Vec3f(0, 0, 1));
        float coneAngle = 1.f - acos(projX) / cone;
        if (coneAngle < 0.f) coneAngle = 0.f;
        if (coneAngle > 1.f) coneAngle = 1.f;
        return angle * intensity * range * coneAngle;
    }
};
