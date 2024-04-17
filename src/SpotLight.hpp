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
        if (point.z > 0.f) return Vec2i(-1, -1);

        Vec2f screen(
            atan2(point.x, -point.z) / PI, // between -1 and 1
            atan2(point.y, sqrt(point.x * point.x + point.z * point.z)) / PI // between -1 and 1
        );
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
        float projX = lightDir.dot(-forward());
        float coneAngle = 1.f - acos(projX) / cone;
        if (coneAngle < 0.f) coneAngle = 0.f;
        if (coneAngle > 1.f) coneAngle = 1.f;
        return angle * intensity * range * coneAngle;
    }
};
