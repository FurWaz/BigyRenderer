#pragma once
#include "Scene.hpp"
#include "FileLoader.hpp"
#include "ModelLoader.hpp"
#include "PointLight.hpp"
#include "DirectionLight.hpp"
#include "SpotLight.hpp"
#include <iostream>

namespace SceneLoader
{
    enum LineType
    {
        MODEL,
        LIGHT,
        COMMENT,
        UNKNOWN
    };

    Scene FromFile(const std::string& path)
    {
        size_t length;
        char* content = FileLoader::ReadFile(path, &length);
        if (content == "") return Scene(); // default to indicate error

        size_t parseStart, parseEnd;
        size_t subParseStart, subParseEnd;

        std::vector<Vec3f> vertices;
        std::vector<Vec2f> textures;
        std::vector<Vec3f> normals;
        std::vector<Triangle> triangles;

        Scene scene;

        LineType type = LineType::UNKNOWN;
        size_t cursor = 0;
        while (cursor < length)
        {
            switch (type)
            {
            case UNKNOWN:
                if (cursor >= length -1) break;
                switch (content[cursor])
                {
                case 'M':
                    type = LineType::MODEL;
                    parseStart = cursor+1;
                    break;
                case 'L':
                    type = LineType::LIGHT;
                    parseStart = cursor+1;
                    break;
                case '#':
                    type = LineType::COMMENT;
                    parseStart = cursor+1;
                    break;
                default: break;
                }
                break;
            
            case COMMENT:
                if (content[cursor] == '\n')
                    type = LineType::UNKNOWN;
                break;
            
            case MODEL:
                if (content[cursor] == '\n')
                {
                    parseEnd = cursor;
                    std::string dataChunk(content+parseStart, parseEnd-parseStart);
                    subParseStart = dataChunk[0] == ' ';
                    subParseEnd = dataChunk.find(' ', subParseStart);
                    std::string modelPath = dataChunk.substr(subParseStart, subParseEnd-subParseStart);
                    Model* m = ModelLoader::FromFolder(modelPath);
                    if (m->valid())
                    {
                        subParseStart = subParseEnd+1;
                        subParseEnd = dataChunk.find(' ', subParseStart);
                        std::string modelPosition = dataChunk.substr(subParseStart, subParseEnd-subParseStart);
                        m->mesh.position = Vec3f(modelPosition);
                        subParseStart = subParseEnd+1;
                        subParseEnd = dataChunk.find(' ', subParseStart);
                        std::string modelRotation = dataChunk.substr(subParseStart, subParseEnd-subParseStart);
                        m->mesh.rotation = Quaternion::Euler(Vec3f(modelRotation));
                        subParseStart = subParseEnd+1;
                        subParseEnd = dataChunk.find(' ', subParseStart);
                        std::string modelScale = dataChunk.substr(subParseStart, subParseEnd-subParseStart);
                        m->mesh.scale = Vec3f(modelScale);
                        scene.addModel(m);
                    }
                    type = LineType::UNKNOWN;
                }
                break;
            
            case LIGHT:
                if (content[cursor] == '\n')
                {
                    parseEnd = cursor;
                    std::string dataChunk(content+parseStart, parseEnd-parseStart);
                    subParseStart = dataChunk[0] == ' ';
                    subParseEnd = dataChunk.find(' ', subParseStart);
                    char lightType = dataChunk[subParseStart];
                    subParseStart = subParseEnd+1;
                    subParseEnd = dataChunk.find(' ', subParseStart);
                    Color col(dataChunk.substr(subParseStart, subParseEnd-subParseStart));
                    subParseStart = subParseEnd+1;
                    subParseEnd = dataChunk.find(' ', subParseStart);
                    float intensity(std::stof(dataChunk.substr(subParseStart, subParseEnd-subParseStart)));
                    subParseStart = subParseEnd+1;
                    subParseEnd = dataChunk.find(' ', subParseStart);
                    float range(std::stof(dataChunk.substr(subParseStart, subParseEnd-subParseStart)));
                    subParseStart = subParseEnd+1;
                    subParseEnd = dataChunk.find(' ', subParseStart);
                    float angle(std::stof(dataChunk.substr(subParseStart, subParseEnd-subParseStart)));
                    subParseStart = subParseEnd+1;
                    subParseEnd = dataChunk.find(' ', subParseStart);
                    Vec3f pos(dataChunk.substr(subParseStart, subParseEnd-subParseStart));
                    subParseStart = subParseEnd+1;
                    subParseEnd = lightType == 's' ? dataChunk.find(' ', subParseStart) : dataChunk.length()-1;
                    Vec3f rot(dataChunk.substr(subParseStart, subParseEnd-subParseStart));
                    Quaternion quatRot = Quaternion::Euler(rot);
                    switch(lightType)
                    {
                        case 'p': // point
                            scene.addLight(new PointLight(pos, quatRot, col, intensity, range));
                            break;
                        case 'd': // direction
                            scene.addLight(new DirectionLight(pos, quatRot, col, intensity, 2.0f));
                            break;
                        case 's': // spot
                            scene.addLight(new SpotLight(pos, quatRot, col, intensity, range, angle));
                            break;
                        case 'a': // ambient
                            scene.setAmbientLight(col);
                            break;
                    }
                    type = LineType::UNKNOWN;
                }
                break;

            default: break;
            }
            cursor++;
        }

        delete[] content;
        return scene;
    }
};
