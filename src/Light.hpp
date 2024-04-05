#pragma once
#include "Vec3.hpp"

class Light
{
public:
    Vec3f position;
    Color color;
    float intensity;
    float range;

    Light();
    Light(Vec3f pos, Color c, float intensity, float range);
    ~Light();
};

Light::Light(): position(Vec3f(0, 0, 0)), color(Color::WHITE), intensity(1), range(1)
{

}

Light::Light(Vec3f pos, Color c, float intensity, float range)
    : position(pos), color(c), intensity(intensity), range(range)
{

}

Light::~Light()
{

}
