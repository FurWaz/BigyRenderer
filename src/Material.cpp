#include "Material.hpp"
#include <iostream>

Material::Material()
{

}

Material::Material(Image diffuse, Image normal, Image specular)
    : diffuse(Image(diffuse)), normal(Image(normal)), specular(Image(specular))
{

}

Material::~Material()
{

}

const bool Material::valid() const
{
    return diffuse.valid() && normal.valid() && specular.valid();
}
