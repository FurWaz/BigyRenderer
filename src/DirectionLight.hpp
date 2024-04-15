#pragma once
#include "Light.hpp"
#include "Model.hpp"
#include "Renderer.hpp"

class DirectionLight : public Light
{
protected:
    float size;

public:
    DirectionLight(Vec3f pos, Quaternion rot, Color col, float intensity, float size)
        : Light(pos, rot, col, intensity), size(size)
    {

    }

    ~DirectionLight()
    {

    }

    Vec2i projectPoint(const Vec3f& point) const
    {
        Vec3f local = rotation.conjugate() * (point - position);
        Vec2f screen(point.x / size, point.y / size);
        return Vec2i(
            (int) ((screen.x + 1) * shadowMapSize / 2),
            (int) ((1 - screen.y) * shadowMapSize / 2)
        );
    }

    float getIntensity(const Vec3f& point, const Vec3f& normal) const
    {
        Vec3f shift = position - point;
        Vec3f lightDir = shift.normalize();
        float angle = lightDir.dot(normal);
        if (angle < 0.f) angle = 0.f;
        return angle * intensity;
    }
};
