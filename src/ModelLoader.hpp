#pragma once
#include <iostream>
#include <vector>
#include "FileLoader.hpp"
#include "Model.hpp"
#include "Timer.hpp"

namespace ModelLoader
{
    enum LineType
    {
        VERTEX,
        TRIANGLE,
        COMMENT,
        UNKNOWN
    };

    Model FromFile(std::string path)
    {
        size_t length;
        char* content = FileLoader::ReadFile(path, &length);
        if (content == "") return Model();

        size_t parseStart, parseEnd;
        size_t subParseStart, subParseEnd;

        std::vector<Vec3f> vertices;
        std::vector<Triangle> triangles;

        Vec3f* verticesArr;
        Triangle* trianglesArr;
        size_t verticesCount;
        size_t trianglesCount;

        LineType type = LineType::UNKNOWN;
        size_t cursor = 0;
        while (cursor < length)
        {
            switch (type)
            {
                case UNKNOWN:
                    if (cursor >= length -1 || content[cursor+1] != ' ') break;
                    switch (content[cursor])
                    {
                    case 'v':
                        type = LineType::VERTEX;
                        parseStart = cursor+2;
                        break;
                    case 'f':
                        type = LineType::TRIANGLE;
                        parseStart = cursor+2;
                        break;
                    case '#':
                    {
                        if (vertices.size() > 0)
                        {
                            verticesCount = vertices.size();
                            verticesArr = new Vec3f[verticesCount];
                            for (size_t i = 0; i < verticesCount; i++)
                                verticesArr[i] = vertices[i];
                        }
                        if (triangles.size() > 0)
                        {
                            trianglesCount = triangles.size();
                            trianglesArr = new Triangle[trianglesCount];
                            for (size_t i = 0; i < trianglesCount; i++)
                                trianglesArr[i] = triangles[i];
                        }
                        type = LineType::COMMENT;
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
                        Vec3f vec;
                        subParseStart = 0;
                        subParseEnd = dataChunk.find(' ', subParseStart);
                        vec.x = std::stof(dataChunk.substr(subParseStart, subParseEnd-subParseStart));
                        subParseStart = subParseEnd+1;
                        subParseEnd = dataChunk.find(' ', subParseStart);
                        vec.y = std::stof(dataChunk.substr(subParseStart, subParseEnd-subParseStart));
                        subParseStart = subParseEnd+1;
                        subParseEnd = dataChunk.find(' ', subParseStart);
                        vec.z = std::stof(dataChunk.substr(subParseStart, subParseEnd-subParseStart));
                        vertices.push_back(vec);
                        type = LineType::UNKNOWN;
                    }
                    break;
                case TRIANGLE:
                    if (content[cursor] == '\n')
                    {
                        parseEnd = cursor;
                        std::string dataChunk(content+parseStart, parseEnd-parseStart);
                        Triangle tri(verticesArr, 0, 0, 0);
                        subParseStart = 0;
                        for (size_t i = 0; i < 3; i++)
                        {
                            subParseEnd = dataChunk.find('/', subParseStart);
                            tri.vertices_index[i] = std::stoi(dataChunk.substr(subParseStart, subParseEnd-subParseStart)) - 1;
                            subParseStart = dataChunk.find(' ', subParseEnd)+1;
                        }
                        triangles.push_back(tri);
                        type = LineType::UNKNOWN;
                    }
                    break;
            }
            cursor++;
        }

        return Model(verticesArr, verticesCount, trianglesArr, trianglesCount);
    }
}
