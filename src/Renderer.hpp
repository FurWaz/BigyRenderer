#pragma once
#include <iostream>
#include <algorithm>
#include <vector>
#include "Camera.hpp"
#include "Image.hpp"
#include "Mesh.hpp"
#include "Material.hpp"
#include "Vec2.hpp"
#include "Light.hpp"

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
        float ratio = im.width / (float) im.height;
        Vec2f screen(
            (point.x / ratio) / -point.z,
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

    void RenderMesh(Image& im, const Camera& cam, const Mesh& mesh, std::vector<Light*> lights)
    {
        Quaternion antiCamRot = cam.rotation.conjugate();

        // calculate projected vertices
        Vec2i* projected = new Vec2i[mesh.n_vertices];
        Vec3f* verts = new Vec3f[mesh.n_vertices];
        Vec3f* norms = new Vec3f[mesh.n_normals];

        for (size_t i = 0; i < mesh.n_vertices; i++)
        {
            // apply mesh rotation + translation
            verts[i] = (mesh.rotation * mesh.vertices[i]) + mesh.position;
            norms[i] = mesh.rotation * mesh.normals[i];

            // apply camera transformation
            verts[i] = antiCamRot * (verts[i] - cam.position);
            norms[i] = antiCamRot * norms[i];

            projected[i] = CameraToScreen(im, verts[i]);
        }

        // render triangles (filling the depth buffer too)
        for (size_t i = 0; i < mesh.n_triangles; i++)
        {
            Triangle& tri = mesh.triangles[i];

            Vec2i p1 = projected[tri.ver_i[0]];
            Vec2i p2 = projected[tri.ver_i[1]];
            Vec2i p3 = projected[tri.ver_i[2]];

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

                    float pixelDepth = bary.x * verts[tri.ver_i[0]].z +
                        bary.y * verts[tri.ver_i[1]].z +
                        bary.z * verts[tri.ver_i[2]].z;
                    pixelDepth = -pixelDepth;

                    if (pixelDepth < 0) continue;
                    if (pixelDepth < im.getDepth(x, y))
                    {
                        Vec3f normal = (norms[tri.nor_i[0]] * bary.x +
                            norms[tri.nor_i[1]] * bary.y +
                            norms[tri.nor_i[2]] * bary.z).normalize();
                        
                        Vec3f position = verts[tri.ver_i[0]] * bary.x +
                            verts[tri.ver_i[1]] * bary.y +
                            verts[tri.ver_i[2]] * bary.z;

                        Vec2f texCoord = mesh.textures[tri.tex_i[0]] * bary.x +
                            mesh.textures[tri.tex_i[1]] * bary.y +
                            mesh.textures[tri.tex_i[2]] * bary.z;

                        const Image& tex = mesh.material.texture;
                        Vec2i imCoord = Vec2i(
                            (int) (texCoord.x * tex.width),
                            (int) ((1.f-texCoord.y) * tex.height)
                        );

                        Color texColor = tex.getPixel(imCoord.x, imCoord.y);
                        Color finalColor(0, 0, 0);

                        // get light intensity
                        for (Light* light : lights)
                        {
                            Vec3f shift = (light->position - position);
                            Vec3f lightDir = shift.normalize();
                            float dist = shift.length();
                            float diff = std::max(0.0f, normal.dot(lightDir));
                            finalColor += texColor * diff * light->intensity * std::clamp(dist / light->range, 0.f, 1.f) * light->color;
                        }

                        im.setDepth(x, y, pixelDepth);
                        im.setPixel(x, y, finalColor);
                    }
                }
            }
        }

        // free memory
        delete[] projected;
        delete[] verts;
        delete[] norms;
    }
}
