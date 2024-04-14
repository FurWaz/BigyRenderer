#include "Material.hpp"
#include <iostream>

Material::Material()
{
    
}

Material::Material(Image diffuse, Image normal, Image specular)
    : diffuse(diffuse), normal(normal), specular(specular)
{

}

Material::~Material()
{

}

bool Material::valid() const
{
    return diffuse.valid() && normal.valid() && specular.valid();
}
