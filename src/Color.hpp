#pragma once

class Color
{
public:
    static const Color TRANSPARENT;
    static const Color BLACK;
    static const Color WHITE;
    static const Color RED;
    static const Color GREEN;
    static const Color BLUE;

    static Color FromArray(unsigned char *data, int channels);
    static Color random();

    unsigned char r, g, b, a;

    Color();
    Color(std::string str);
    Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255);
    Color(const Color &other);
    ~Color();
    
    Color operator*(const Color& other) const;
    Color operator+=(const Color& other);
    Color operator*(float scalar) const;
    Color operator/(float scalar) const;

    Color hover(const Color& other) const;
    Color mix(const Color& other, float ratio) const;
    void toArray(unsigned char *data, int index) const;
};
