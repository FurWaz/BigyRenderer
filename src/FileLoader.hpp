#pragma once
#include <fstream>

namespace FileLoader
{
    char* ReadFile(const std::string &path, size_t* length)
    {
        std::ifstream file(path, std::ios::binary);
        if (!file.is_open()) return nullptr;

        file.seekg(0, std::ios::end);
        *length = file.tellg();
        file.seekg(0, std::ios::beg);

        char* content = new char[*length];
        file.read(content, *length);
        file.close();

        return content;
    }
}
