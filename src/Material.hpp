#pragma once
#include "Image.hpp"

class Material
{
public:
    Image diffuse;
    Image normal;

    Material();
    Material(Image diffuse, Image normal);
    ~Material();

    const bool valid() const;
};
