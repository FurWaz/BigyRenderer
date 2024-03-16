#include <iostream>
#include "Image.hpp"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image.h"
#include "stb/stb_image_write.h"

Image::Image(): data(nullptr), width(0), height(0) {}

Image::Image(int width, int height)
    : width(width), height(height), data(new Color[width * height]), path("")
{
    memset(data, 0, width * height * sizeof(Color));
}

Image::Image(std::string path)
    : data(nullptr), width(0), height(0), path(path)
{

}

Image::Image(const Image &other)
    : data(nullptr), width(other.width), height(other.height), path(other.path)
{
    if (other.data)
    {
        data = new Color[width * height];
        for (int i = 0; i < width * height; i++)
        {
            data[i] = other.data[i];
        }
    }
}

Image::~Image()
{
    if (data) delete[] data;
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
        for (int i = 0; i < width * height; i++)
        {
            this->data[i] = Color::FromArray(data + i * 4, 4);
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
        data[i] = Color(color);
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
