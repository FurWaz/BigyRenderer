#include "Model.hpp"

Model::Model()
{
    
}

Model::Model(Mesh mesh, Material material)
    : mesh(mesh), material(material)
{

}

Model::Model(const Model &other)
    : mesh(other.mesh), material(other.material)
{

}

Model::~Model()
{

}

const bool Model::valid() const
{
    return mesh.valid() && material.valid();
}
