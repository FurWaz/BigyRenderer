#pragma once
#include "Vec3.hpp"
#include "Color.hpp"
#include "Object.hpp"

class Light : public Object
{
protected:
    Light();
    Light(Vec3f pos, Quaternion rot, Color c, float intensity);

public:
    Color color;
    float intensity;
    
    virtual ~Light();

    virtual float getIntensity(Vec3f point, Vec3f normal) const = 0;
    const Color& getColor() const;
};

Light::Light()
    : Object(), color(Color::WHITE), intensity(1.0)
{

}

Light::Light(Vec3f pos, Quaternion rot, Color c, float intensity)
    : Object(pos, rot), color(c), intensity(intensity)
{

}

Light::~Light()
{

}

const Color& Light::getColor() const
{
    return color;
}
