#pragma once
#include <iostream>
#include <vector>
#include "FileLoader.hpp"
#include "Mesh.hpp"
#include "Timer.hpp"

namespace MeshLoader
{
    enum LineType
    {
        VERTEX,
        TEXTURE,
        NORMAL,
        TRIANGLE,
        COMMENT,
        UNKNOWN
    };

    Mesh FromFile(std::string path)
    {
        size_t length;
        char* content = FileLoader::ReadFile(path, &length);
        if (content == "") return Mesh();

        size_t parseStart, parseEnd;
        size_t subParseStart, subParseEnd;

        std::vector<Vec3f> vertices;
        std::vector<Vec2f> textures;
        std::vector<Vec3f> normals;
        std::vector<Triangle> triangles;

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
                    case '#':
                        type = LineType::COMMENT;
                        parseStart = cursor+1;
                        break;
                    case 'f':
                        type = LineType::TRIANGLE;
                        parseStart = cursor+2;
                        break;
                    case 'v':
                    {
                        switch (content[cursor+1])
                        {
                            case ' ':
                                type = LineType::VERTEX;
                                parseStart = cursor+2;
                                break;
                            case 'n':
                                type = LineType::NORMAL;
                                parseStart = cursor+3;
                                break;
                            case 't':
                                type = LineType::TEXTURE;
                                parseStart = cursor+3;
                                break;
                            default:
                                break;
                        }
                        break;
                    }
                    default:
                        break;
                    }
                    cursor++;
                    break;
                case COMMENT:
                    if (content[cursor] == '\n')
                        type = LineType::UNKNOWN;
                    break;
                case VERTEX:
                    if (content[cursor] == '\n')
                    {
                        parseEnd = cursor;
                        std::string dataChunk(content+parseStart, parseEnd-parseStart);
                        Vec3f ver;
                        subParseStart = dataChunk[0] == ' ';
                        subParseEnd = dataChunk.find(' ', subParseStart);
                        ver.x = std::stof(dataChunk.substr(subParseStart, subParseEnd-subParseStart));
                        subParseStart = subParseEnd+1;
                        subParseEnd = dataChunk.find(' ', subParseStart);
                        ver.y = std::stof(dataChunk.substr(subParseStart, subParseEnd-subParseStart));
                        subParseStart = subParseEnd+1;
                        subParseEnd = dataChunk.find(' ', subParseStart);
                        ver.z = std::stof(dataChunk.substr(subParseStart, subParseEnd-subParseStart));
                        vertices.push_back(ver);
                        type = LineType::UNKNOWN;
                    }
                    break;
                case TEXTURE:
                    if (content[cursor] == '\n')
                    {
                        parseEnd = cursor;
                        std::string dataChunk(content+parseStart, parseEnd-parseStart);
                        Vec2f tex;
                        subParseStart = dataChunk[0] == ' ';
                        subParseEnd = dataChunk.find(' ', subParseStart);
                        tex.x = std::stof(dataChunk.substr(subParseStart, subParseEnd-subParseStart));
                        subParseStart = subParseEnd+1;
                        subParseEnd = dataChunk.find(' ', subParseStart);
                        tex.y = std::stof(dataChunk.substr(subParseStart, subParseEnd-subParseStart));
                        textures.push_back(tex);
                        type = LineType::UNKNOWN;
                    }
                    break;
                case NORMAL:
                    if (content[cursor] == '\n')
                    {
                        parseEnd = cursor;
                        std::string dataChunk(content+parseStart, parseEnd-parseStart);
                        Vec3f nor;
                        subParseStart = dataChunk[0] == ' ';
                        subParseEnd = dataChunk.find(' ', subParseStart);
                        nor.x = std::stof(dataChunk.substr(subParseStart, subParseEnd-subParseStart));
                        subParseStart = subParseEnd+1;
                        subParseEnd = dataChunk.find(' ', subParseStart);
                        nor.y = std::stof(dataChunk.substr(subParseStart, subParseEnd-subParseStart));
                        subParseStart = subParseEnd+1;
                        subParseEnd = dataChunk.find(' ', subParseStart);
                        nor.z = std::stof(dataChunk.substr(subParseStart, subParseEnd-subParseStart));
                        normals.push_back(nor);
                        type = LineType::UNKNOWN;
                    }
                    break;
                case TRIANGLE:
                    if (content[cursor] == '\n')
                    {
                        parseEnd = cursor;
                        std::string dataChunk(content+parseStart, parseEnd-parseStart);
                        Triangle tri;
                        subParseStart = dataChunk[0] == ' ';
                        for (size_t i = 0; i < 3; i++)
                        {
                            subParseEnd = dataChunk.find('/', subParseStart);
                            tri.ver_i[i] = std::stoi(dataChunk.substr(subParseStart, subParseEnd-subParseStart)) - 1;

                            subParseStart = subParseEnd+1;
                            subParseEnd = dataChunk.find('/', subParseStart);
                            tri.tex_i[i] = std::stoi(dataChunk.substr(subParseStart, subParseEnd-subParseStart)) - 1;

                            subParseStart = subParseEnd+1;
                            subParseEnd = dataChunk.find(' ', subParseStart);
                            tri.nor_i[i] = std::stoi(dataChunk.substr(subParseStart, subParseEnd-subParseStart)) - 1;

                            subParseStart = subParseEnd;
                        }
                        triangles.push_back(tri);
                        type = LineType::UNKNOWN;
                    }
                    break;
            }
            cursor++;
        }

        size_t verticesCount = vertices.size();
        Vec3f* verticesArr = new Vec3f[verticesCount];
        for (size_t i = 0; i < verticesCount; i++)
            verticesArr[i] = Vec3f(vertices[i]);

        size_t texturesCount = textures.size();
        Vec2f* texturesArr = new Vec2f[texturesCount];
        for (size_t i = 0; i < texturesCount; i++)
            texturesArr[i] = Vec2f(textures[i]);

        size_t normalsCount = normals.size();
        Vec3f* normalsArr = new Vec3f[normalsCount];
        for (size_t i = 0; i < normalsCount; i++)
            normalsArr[i] = Vec3f(normals[i]);
        
        size_t trianglesCount = triangles.size();
        Triangle* trianglesArr = new Triangle[trianglesCount];
        for (size_t i = 0; i < trianglesCount; i++)
            trianglesArr[i] = triangles[i];

        return Mesh(
            verticesArr, verticesCount,
            texturesArr, texturesCount,
            normalsArr, normalsCount,
            trianglesArr, trianglesCount
        );

        delete[] verticesArr;
        delete[] texturesArr;
        delete[] normalsArr;
        delete[] trianglesArr;
    }
}
