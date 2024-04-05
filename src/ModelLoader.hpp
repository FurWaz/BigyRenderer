#pragma once
#include <string>
#include <filesystem>
#include <map>
#include <iostream>
#include "Model.hpp"
#include "MeshLoader.hpp"

namespace ModelLoader
{
    Model FromFolder(std::string folderPath)
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
        
        std::string required[] = {"mesh", "diffuse", "normal"};
        for (const std::string& name : required)
        {
            if (files.find(name) == files.end())
            {
                std::cerr << "Error: Missing " << name << " file" << std::endl;
                return Model(); // invalid model to indicate failure
            }
        }

        std::string meshPath = files["mesh"];
        std::string diffusePath = files["diffuse"];
        std::string normalPath = files["normal"];

        Mesh mesh = MeshLoader::FromFile(meshPath);
        if (!mesh.valid())
        {
            std::cerr << "Error: Invalid mesh" << std::endl;
            return Model(); // invalid model to indicate failure
        }

        Image diffuse(diffusePath, true);
        if (!diffuse.valid())
        {
            std::cerr << "Error: Invalid diffuse image" << std::endl;
            return Model(); // invalid model to indicate failure
        }

        Image normal(normalPath, true);
        if (!normal.valid())
        {
            std::cerr << "Error: Invalid normal image" << std::endl;
            return Model(); // invalid model to indicate failure
        }
        
        Material material(diffuse, normal);

        return Model(mesh, material);
    }
}
