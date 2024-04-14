#pragma once
#include <string>
#include <filesystem>
#include <map>
#include <iostream>
#include "Model.hpp"
#include "MeshLoader.hpp"

#define fail(x) { std::cerr << x << std::endl; return new Model(); }

namespace ModelLoader
{
    Model* FromFolder(std::string folderPath)
    {
        auto folders = std::filesystem::directory_iterator(folderPath);
        std::map<std::string, std::string> files;

        for (const auto & entry : folders)
        {
            std::string filename = entry.path().filename().string();
            std::string extension = entry.path().extension().string();

            std::string filetype = filename.substr(0, filename.size() - extension.size());
            files[filetype] = entry.path().string();
        }
        
        std::string required[] = {"mesh", "diffuse", "normal", "specular"};
        for (const std::string& name : required)
        {
            if (files.find(name) == files.end())
                fail("Error: Missing " << name << " file");
        }

        std::string meshPath = files["mesh"];
        std::string diffusePath = files["diffuse"];
        std::string normalPath = files["normal"];
        std::string specularPath = files["specular"];

        Mesh mesh = MeshLoader::FromFile(meshPath);
        if (!mesh.valid())
            fail("Error: Invalid mesh");

        Image diffuse(diffusePath, true);
        if (!diffuse.valid())
            fail("Error: Invalid diffuse image");

        Image normal(normalPath, true);
        if (!normal.valid())
            fail("Error: Invalid normal image");

        Image specular(specularPath, true);
        if (!specular.valid())
            fail("Error: Invalid specular image");
        
        Material material(diffuse, normal, specular);

        return new Model(mesh, material);
    }
}
