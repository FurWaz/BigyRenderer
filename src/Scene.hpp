#pragma once
#include <vector>
#include "Model.hpp"
#include "Light.hpp"

class Scene
{
private:
    std::vector<Model*> models;
    std::vector<Light*> lights;

public:
    Scene() {}
    ~Scene() {}

    void addModel(Model model)
    {
        models.push_back(new Model(model));
    }

    void addLight(Light light)
    {
        lights.push_back(new Light(light));
    }

    bool empty()
    {
        return (models.size() + lights.size()) == 0;
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
