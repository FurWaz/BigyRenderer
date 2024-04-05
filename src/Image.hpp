#pragma once
#include <string>
#include "Color.hpp"

class Image
{
private:
    Color* data;
    float* depth;

public:
    int width, height;
    std::string path;
    
    Image();
    Image(int width, int height);
    Image(std::string path, bool load = false);
    Image(const Image &other);
    ~Image();

    const bool valid() const;

    void save(std::string path = "");
    void saveDepth(std::string path = "");
    void load(std::string path = "");
    void clear(const Color &color = Color::BLACK);

    bool pixelCheck(int x, int y) const;

    Color getPixel(int x, int y) const;
    void setPixel(int x, int y, const Color &color);

    float getDepth(int x, int y) const;
    void setDepth(int x, int y, float depth);
};
