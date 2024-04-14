#pragma once
#include "Image.hpp"

class Material
{
public:
    Image diffuse;
    Image normal;
    Image specular;

    Material();
    Material(Image diffuse, Image normal, Image specular);
    ~Material();

    bool valid() const;
};
