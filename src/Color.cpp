#include <iostream>
#include <string>
#include <algorithm>
#include "Color.hpp"

const Color Color::TRANSPARENT = Color(0, 0, 0, 0);
const Color Color::BLACK = Color(0, 0, 0);
const Color Color::WHITE = Color(255, 255, 255);
const Color Color::RED = Color(255, 0, 0);
const Color Color::GREEN = Color(0, 255, 0);
const Color Color::BLUE = Color(0, 0, 255);

Color Color::FromArray(unsigned char *data, int channels)
{
    switch (channels)
    {
        case 3: return Color(data[0], data[1], data[2]);
        case 4: return Color(data[0], data[1], data[2], data[3]);
        default:
            std::cerr << "Error : Unsupported number of channels! (" << channels << ")" << std::endl;
            return Color::BLACK;
    }
}

Color Color::random()
{
    return Color(
        (unsigned char) (rand() % 256),
        (unsigned char) (rand() % 256),
        (unsigned char) (rand() % 256)
    );
}

Color::Color()
    : r(0), g(0), b(0), a(255)
{

}

Color::Color(std::string str)
{
    size_t start = 0;
    size_t end = str.find(",");
    this->r = std::stoi(str.substr(start, end-start));
    start = end;
    end = str.find(",", start+1);
    this->g = std::stoi(str.substr(start+1, end-start));
    start = end;
    end = str.length()-1;
    this->b = std::stoi(str.substr(start+1, end-start));
    this->a = 255;
}

Color::Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
    : r(r), g(g), b(b), a(a)
{

}

Color::Color(const Color &other)
    : r(other.r), g(other.g), b(other.b), a(other.a)
{

}

Color::~Color()
{

}

Color Color::operator*(const Color& other) const
{
    return Color(
        (unsigned char) ((float) r * other.r / 255.0f),
        (unsigned char) ((float) g * other.g / 255.0f),
        (unsigned char) ((float) b * other.b / 255.0f),
        (unsigned char) ((float) a * other.a / 255.0f)
    );
}

Color Color::operator+=(const Color& other)
{
    r = std::min((int) r + other.r, 255);
    g = std::min((int) g + other.g, 255);
    b = std::min((int) b + other.b, 255);
    a = std::min((int) a + other.a, 255);

    return *this;
}

Color Color::operator*(float scalar) const
{
    return Color(
        (unsigned char) (std::min((float) r * scalar, 255.0f)),
        (unsigned char) (std::min((float) g * scalar, 255.0f)),
        (unsigned char) (std::min((float) b * scalar, 255.0f)),
        a
    );
}

Color Color::operator/(float scalar) const
{
    return Color(
        (unsigned char) (r / scalar),
        (unsigned char) (g / scalar),
        (unsigned char) (b / scalar),
        a
    );
}

Color Color::hover(const Color &other) const
{
    float alpha = other.a / 255.0f;
    float inv_alpha = 1.0f - alpha;

    return Color(
        (unsigned char) (r * inv_alpha + other.r * alpha),
        (unsigned char) (g * inv_alpha + other.g * alpha),
        (unsigned char) (b * inv_alpha + other.b * alpha),
        (unsigned char) (a * inv_alpha + other.a * alpha)
    );
}

Color Color::mix(const Color &other, float ratio) const
{
    float inv_ratio = 1.0f - ratio;

    return Color(
        (unsigned char) (r * inv_ratio + other.r * ratio),
        (unsigned char) (g * inv_ratio + other.g * ratio),
        (unsigned char) (b * inv_ratio + other.b * ratio),
        (unsigned char) (a * inv_ratio + other.a * ratio)
    );
}

void Color::toArray(unsigned char *data, int index) const
{
    data[index + 0] = r;
    data[index + 1] = g;
    data[index + 2] = b;
    data[index + 3] = a;
}