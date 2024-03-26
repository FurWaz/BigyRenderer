#pragma once
#include <iostream>
#include <algorithm>
#include "Camera.hpp"
#include "Image.hpp"
#include "Model.hpp"
#include "Vec2.hpp"

Vec3f barycentric(Vec2i p1, Vec2i p2, Vec2i p3, Vec2i p)
{
    Vec3i u = Vec3i(p3.x - p1.x, p2.x - p1.x, p1.x - p.x);
    Vec3i v = Vec3i(p3.y - p1.y, p2.y - p1.y, p1.y - p.y);
    Vec3i w = u.cross(v);

    if (std::abs(w.z) < 1) return Vec3f(-1, 1, 1);

    return Vec3f(1.0f - (w.x + w.y) / (float)w.z, w.y / (float)w.z, w.x / (float)w.z);
}

namespace Renderer
{
    Vec2i CameraToScreen(const Image &im, const Vec3f& point)
    {
        Vec2f screen(
            point.x / -point.z,
            point.y / -point.z
        );
        return Vec2i(
            (int) ((screen.x + 1) * im.width / 2),
            (int) ((1 - screen.y) * im.height / 2)
        );
    }

    void DisplayPoint(Image& im, Vec2i p, Color c)
    {
        if (im.pixelCheck(p.x, p.y))
            im.setPixel(p.x, p.y, im.getPixel(p.x, p.y).hover(c));
    }

    void DisplayLine(Image &im, Vec2i p1, Vec2i p2, const Color &color)
    {
        int x1 = p1.x, y1 = p1.y, x2 = p2.x, y2 = p2.y;
        int dx = abs(x2 - x1), dy = abs(y2 - y1);
        int sx = (x1 < x2) ? 1 : -1, sy = (y1 < y2) ? 1 : -1;
        int err = dx - dy;

        while (true)
        {
            DisplayPoint(im, Vec2i(x1, y1), color);
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

    void RenderModel(Image& im, const Camera& cam, const Model& model)
    {
        // calculate projected vertices
        Vec2i* projected = new Vec2i[model.n_vertices];
        Point* points = new Point[model.n_vertices];
        for (size_t i = 0; i < model.n_vertices; i++)
        {
            // apply model rotation + translation
            points[i] = (model.rotation * model.vertices[i]);
            points[i].position += model.position;

            // apply camera transformation
            Quaternion antiCamRot = cam.rotation.conjugate();
            points[i].position = antiCamRot * (points[i].position - cam.position);
            points[i].normal = antiCamRot * points[i].normal;

            projected[i] = CameraToScreen(im, points[i].position);
        }

        // render triangles (filling the depth buffer too)
        for (size_t i = 0; i < model.n_triangles; i++)
        {
            Triangle& tri = model.triangles[i];
            Vec3f normal = (points[tri.vert_i[1]].position - points[tri.vert_i[0]].position).normalize()
                .cross((points[tri.vert_i[2]].position - points[tri.vert_i[0]].position).normalize());

            // NOTE : Commented because o artifacts when weird rotations
            // float orient = cam.forward().dot(normal);
            // if (orient < 0.f) continue;

            Vec2i p1 = projected[tri.vert_i[0]];
            Vec2i p2 = projected[tri.vert_i[1]];
            Vec2i p3 = projected[tri.vert_i[2]];

            Vec2i b1(
                std::min(std::min(p1.x, p2.x), p3.x),
                std::min(std::min(p1.y, p2.y), p3.y)
            );
            Vec2i b2(
                std::max(std::max(p1.x, p2.x), p3.x),
                std::max(std::max(p1.y, p2.y), p3.y)
            );

            bool outOfImage = !im.pixelCheck(b1.x, b1.y) && !im.pixelCheck(b2.x, b2.y);
            if (outOfImage) continue;

            for (int x = b1.x; x < b2.x; x++)
            {
                for (int y = b1.y; y < b2.y; y++)
                {
                    Vec2i p(x, y);
                    if (!im.pixelCheck(x, y)) continue;

                    Vec3f bary = barycentric(p1, p2, p3, p);
                    if (bary.x < 0 || bary.y < 0 || bary.z < 0) continue;

                    float pixelDepth = bary.x * points[tri.vert_i[0]].position.z +
                        bary.y * points[tri.vert_i[1]].position.z +
                        bary.z * points[tri.vert_i[2]].position.z;
                    pixelDepth = -pixelDepth;

                    if (pixelDepth < 0) continue;
                    if (pixelDepth < im.getDepth(x, y))
                    {
                        Color color(
                            (unsigned char) (((normal.x + 1) / 2) * 255.f), 
                            (unsigned char) (((normal.y + 1) / 2) * 255.f), 
                            (unsigned char) (((normal.z + 1) / 2) * 255.f)
                        );
                        im.setDepth(x, y, pixelDepth);
                        im.setPixel(x, y, color);
                    }
                }
            }
        }

        // free memory
        delete[] projected;
        delete[] points;
    }
}
