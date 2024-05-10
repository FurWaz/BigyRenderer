#include <iostream>
#include "Image.hpp"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image.h"
#include "stb/stb_image_write.h"

void Image::saveDepth(const std::string& path, int width, int height, float* data)
{
    unsigned char* img = new unsigned char[width * height * 4];
    for (int i = 0; i < width * height; i++)
    {
        float depth = data[i];
        int value = (int)(depth * 64);
        if (value < 0) value = 0;
        if (value > 255) value = 255;
        img[i * 4] = value;
        img[i * 4 + 1] = value;
        img[i * 4 + 2] = value;
        img[i * 4 + 3] = 255;
    }

    stbi_write_png(path.c_str(), width, height, 4, img, width * 4);
    delete[] img;
}

Image::Image(): data(nullptr), width(0), height(0) {}

Image::Image(int width, int height)
    : width(width), height(height), data(new Color[width * height]), depth(new float[width * height]), path("")
{
    memset(data, 0, width * height * sizeof(Color));
    for (int i = 0; i < width * height; i++)
        depth[i] = std::numeric_limits<float>::infinity();
}

Image::Image(std::string path, bool load)
    : data(nullptr), depth(nullptr), width(0), height(0), path(path)
{
    if (load) this->load();
}

Image::Image(const Image &other)
    : data(new Color[other.width * other.height]),
      depth(new float[other.width * other.height]),
      width(other.width), height(other.height), path(other.path)
{
    for (size_t i = 0; i < width * height; i++)
        data[i] = Color(other.data[i]);
    for (size_t i = 0; i < width * height; i++)
        depth[i] = other.depth[i];
}

Image::~Image()
{
    if (data) delete[] data;
    if (depth) delete[] depth;
}

bool Image::valid() const
{
    return width > 0 && height > 0;
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
    delete[] data;
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
        if (depth <= 0 || depth >= 4)
            Color(0, 0, 0, 255).toArray(data, i * 4);
        else
        {
            unsigned char c = (unsigned char) (int) (depth * 64);
            Color(c, c, c, 255).toArray(data, i * 4);
        }
    }

    stbi_write_png(path.c_str(), width, height, 4, data, width * 4);
    delete[] data;
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
        if (this->data) delete[] data;
        if (this->depth) delete[] depth;

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
        data[i].r = color.r;
        data[i].g = color.g;
        data[i].b = color.b;
        data[i].a = color.a;
        depth[i] = std::numeric_limits<float>::infinity();
    }
}

bool Image::pixelCheck(int x, int y) const
{
    return x >= 0 && x < width && y >= 0 && y < height;
}

const Color& Image::getPixel(int x, int y) const
{
    // if (!pixelCheck(x, y))
    //     return Color::BLACK;
    return data[x + y * width];
}

void Image::setPixel(int x, int y, const Color &color)
{
    // if (!pixelCheck(x, y)) return;
    data[x + y * width] = Color(color);
}

float Image::getDepth(int x, int y) const
{
    // if (!pixelCheck(x, y))
    //     return std::numeric_limits<float>::infinity();
    return depth[x + y * width];
}

void Image::setDepth(int x, int y, float depth)
{
    // if (!pixelCheck(x, y)) return;
    this->depth[x + y * width] = depth;
}
