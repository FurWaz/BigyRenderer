#include <iostream>
#include "Image.hpp"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image.h"
#include "stb/stb_image_write.h"

Image::Image(): data(nullptr), width(0), height(0) {}

Image::Image(int width, int height)
    : width(width), height(height), data(new Color[width * height]), depth(new float[width * height]), path("")
{
    memset(data, 0, width * height * sizeof(Color));
    for (int i = 0; i < width * height; i++)
        depth[i] = std::numeric_limits<float>::infinity();
}

Image::Image(std::string path)
    : data(nullptr), depth(nullptr), width(0), height(0), path(path)
{

}

Image::Image(const Image &other)
    : data(nullptr), depth(nullptr), width(other.width), height(other.height), path(other.path)
{
    if (other.data)
    {
        data = new Color[width * height];
        for (int i = 0; i < width * height; i++)
        {
            data[i] = other.data[i];
            depth[i] = other.depth[i];
        }
    }
}

Image::~Image()
{
    if (data) delete[] data;
    if (depth) delete[] depth;
}

void Image::save(std::string path)
{
    if (path == "") path = this->path;
    if (path == "")
    {
        std::cerr << "Error : No path specified for saving image!" << std::endl;
        return;
    }

    unsigned char* data = new unsigned char[width * height * 4];
    for (int i = 0; i < width * height; i++)
        this->data[i].toArray(data, i * 4);

    stbi_write_png(path.c_str(), width, height, 4, data, width * 4);
}

void Image::saveDepth(std::string path)
{
    if (path == "") path = this->path;
    if (path == "")
    {
        std::cerr << "Error : No path specified for saving depth image!" << std::endl;
        return;
    }

    unsigned char* data = new unsigned char[width * height * 4];
    for (int i = 0; i < width * height; i++)
    {
        float depth = this->depth[i];
        if (depth <= 0 || depth >= 2)
            Color(0, 0, 0, 255).toArray(data, i * 4);
        else
        {
            unsigned char c = (unsigned char) (int) (depth * 128);
            Color(c, c, c, 255).toArray(data, i * 4);
        }
    }

    stbi_write_png(path.c_str(), width, height, 4, data, width * 4);
}

void Image::load(std::string path)
{
    if (path == "") path = this->path;
    if (path == "")
    {
        std::cerr << "Error : No path specified for loading image!" << std::endl;
        return;
    }

    int channels, width, height;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 4);

    if (data)
    {
        this->data = new Color[width * height];
        this->depth = new float[width * height];
        for (int i = 0; i < width * height; i++)
        {
            this->data[i] = Color::FromArray(data + i * 4, 4);
            this->depth[i] = std::numeric_limits<float>::infinity();
        }
        this->path = path;
        this->width = width;
        this->height = height;
        stbi_image_free(data);
    }
    else
    {
        std::cerr << "Error : Failed to load image! (" << this->path << ")" << std::endl;
    }
}

void Image::clear(const Color &color)
{
    for (int i = 0; i < width * height; i++)
    {
        data[i] = Color(color);
        depth[i] = std::numeric_limits<float>::infinity();
    }
}

bool Image::pixelCheck(int x, int y) const
{
    return x >= 0 && x < width && y >= 0 && y < height;
}

Color Image::getPixel(int x, int y) const
{
    return data[x + y * width];
}

void Image::setPixel(int x, int y, const Color &color)
{
    data[x + y * width] = Color(color);
}

float Image::getDepth(int x, int y) const
{
    return depth[x + y * width];
}

void Image::setDepth(int x, int y, float depth)
{
    this->depth[x + y * width] = depth;
}
