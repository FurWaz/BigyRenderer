#pragma once
#include "Mesh.hpp"
#include "Material.hpp"

class Model
{
public:
    Mesh mesh;
    Material material;

    Model();
    Model(Mesh mesh, Material material);
    Model(const Model &other);
    ~Model();

    const bool valid() const;
};
