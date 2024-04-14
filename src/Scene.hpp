#pragma once
#include <vector>
#include "Model.hpp"
#include "Light.hpp"

class Scene
{
private:
    std::vector<Model*> models;
    std::vector<Light*> lights;
    float ambientLight;

public:
    Scene() {}
    ~Scene() {}

    void addModel(Model* model)
    {
        models.push_back(model);
    }

    void addLight(Light* light)
    {
        lights.push_back(light);
    }

    bool empty()
    {
        return (models.size() + lights.size()) == 0;
    }

    void setAmbientLight(float intensity)
    {
        ambientLight = intensity;
    }

    float getAmbientLight() const
    {
        return ambientLight;
    }

    const std::vector<Model*>& getModels() const
    {
        return models;
    }

    const std::vector<Light*>& getLights() const
    {
        return lights;
    }
};
