#pragma once
#include <math.h>
#include "Image.hpp"
#include "Model.hpp"
#include "Vec2.hpp"

namespace Draw
{
    void Point(Image &im, Vec2i p, const Color &color)
    {
        if (im.pixelCheck(p.x, p.y))
            im.setPixel(p.x, p.y, im.getPixel(p.x, p.y).hover(color));
    }

    void Line(Image &im, Vec2i p1, Vec2i p2, const Color &color)
    {
        int x1 = p1.x, y1 = p1.y, x2 = p2.x, y2 = p2.y;
        int dx = abs(x2 - x1), dy = abs(y2 - y1);
        int sx = (x1 < x2) ? 1 : -1, sy = (y1 < y2) ? 1 : -1;
        int err = dx - dy;

        while (true)
        {
            Point(im, Vec2i(x1, y1), color);
            if (x1 == x2 && y1 == y2) break;
            int e2 = 2 * err;
            if (e2 > -dy)
            {
                err -= dy;
                x1 += sx;
            }
            if (e2 < dx)
            {
                err += dx;
                y1 += sy;
            }
        }
    }
}
