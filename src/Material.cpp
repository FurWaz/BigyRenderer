#include "Material.hpp"
#include <iostream>

Material::Material()
{

}

Material::Material(Image diffuse, Image normal)
    : diffuse(Image(diffuse)), normal(Image(normal))
{

}

Material::~Material()
{

}

const bool Material::valid() const
{
    return diffuse.valid() && normal.valid();
}
