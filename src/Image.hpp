#pragma once
#include <string>
#include "Color.hpp"

class Image
{
private:
    Color* data;

public:
    int width, height;
    std::string path;
    
    Image();
    Image(int width, int height);
    Image(std::string path);
    Image(const Image &other);
    ~Image();

    void save(std::string path = "");
    void load(std::string path = "");
    void clear(const Color &color = Color::BLACK);

    bool pixelCheck(int x, int y) const;

    Color getPixel(int x, int y) const;
    void setPixel(int x, int y, const Color &color);
};
