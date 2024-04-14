#pragma once
#include <string>
#include <string.h>
#include <iostream>
#include "tinydir/tinydir.h"
#include "Model.hpp"
#include "MeshLoader.hpp"

#define fail(x) { std::cerr << x << std::endl; return new Model(); }

namespace ModelLoader
{
    bool strprefix(const char *str, const char *pre)
    {
        return strncmp(pre, str, strlen(pre)) == 0;
    }

    Model* FromFolder(std::string folderPath)
    {
        std::string meshPath;
        std::string normalPath;
        std::string diffusePath;
        std::string specularPath;

        tinydir_dir dir;
        int i;
        tinydir_open_sorted(&dir, folderPath.c_str());
        for (i = 0; i < dir.n_files; i++)
        {
            tinydir_file file;
            tinydir_readfile_n(&dir, &file, i);

            if (strprefix(file.name, "diffuse"))
                diffusePath = file.path;
            if (strprefix(file.name, "specular"))
                specularPath = file.path;
            if (strprefix(file.name, "normal"))
                normalPath = file.path;
            if (strprefix(file.name, "mesh"))
                meshPath = file.path;
        }
        tinydir_close(&dir);
        
        Mesh mesh = MeshLoader::FromFile(meshPath);
        if (!mesh.valid()) fail("Error : Invalid mesh");

        Image normalImg(normalPath, true);
        Image diffuseImg(diffusePath, true);
        Image specularImg(specularPath, true);

        if (!normalImg.valid()) fail("Error : Invalid normal image");
        if (!diffuseImg.valid()) fail("Error : Invalid diffuse image");
        if (!specularImg.valid()) fail("Error : Invalid specular image");
        
        Material mat(diffuseImg, normalImg, specularImg);
        return new Model(mesh, mat);
    }
}
