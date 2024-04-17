#pragma once
#include <iostream>
#include <algorithm>
#include <vector>
#include "Camera.hpp"
#include "Image.hpp"
#include "Model.hpp"
#include "Material.hpp"
#include "Vec2.hpp"
#include "Light.hpp"
#include "Scene.hpp"
#include "Utils.hpp"

namespace Renderer
{
    Vec2i ProjectPoint(int width, int height, const Vec3f& point)
    {
        float ratio = width / (float) height;
        Vec2f screen(
            (point.x / ratio) / -point.z,
            point.y / -point.z
        );
        return Vec2i(
            (int) ((screen.x + 1) * width / 2),
            (int) ((1 - screen.y) * height / 2)
        );
    }

    void DisplayPoint(Image& im, Vec2i p, Color c)
    {
        if (im.pixelCheck(p.x, p.y))
            im.setPixel(p.x, p.y, c);
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

    void DisplayCircle(Image& im, Vec2i p, float radius, const Color& color)
    {
        Vec2i min = Vec2i(std::max(0, p.x - (int) radius),        std::max(0, p.y - (int) radius));
        Vec2i max = Vec2i(std::min(im.width, p.x + (int) radius), std::min(im.height, p.y + (int) radius));

        for (int x = min.x; x < max.x; x++)
        {
            for (int y = min.y; y < max.y; y++)
            {
                Vec2i p2(x, y);
                if ((p2 - p).length() > radius || !im.pixelCheck(p2.x, p2.y))
                    continue;
                
                DisplayPoint(im, p2, color);
            }
        }
    }

    void RenderModel(Image& im, const Camera& cam, const Model& model, Color ambientLight, const std::vector<Light*>& lights)
    {
        Quaternion antiCamRot = cam.rotation.conjugate();
        const Mesh& mesh = model.mesh;

        // calculate projected vertices
        Vec3f* verts = new Vec3f[mesh.n_vertices];
        Vec3f* norms = new Vec3f[mesh.n_normals];
        Vec2i* projected = new Vec2i[mesh.n_vertices];
        Vec3f* vertsCamera = new Vec3f[mesh.n_vertices];

        for (size_t i = 0; i < mesh.n_vertices; i++) // vertex transformation
        {
            // apply mesh rotation + translation
            verts[i] = (mesh.rotation * mesh.vertices[i]) + mesh.position;
            // apply camera transformation
            vertsCamera[i] = antiCamRot * (verts[i] - cam.position);
            // project to screen
            projected[i] = ProjectPoint(im.width, im.height, vertsCamera[i]);
        }
        for (size_t i = 0; i < mesh.n_normals; i++) // normal transformation
        {
            // apply mesh rotation
            norms[i] = mesh.rotation * mesh.normals[i];
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

            for (int x = b1.x; x < b2.x; x++)
            {
                for (int y = b1.y; y < b2.y; y++)
                {
                    Vec2i p(x, y);
                    if (!im.pixelCheck(x, y)) continue;

                    Vec3f bary = Utils::barycentric(p1, p2, p3, p);
                    if (bary.x < 0 || bary.y < 0 || bary.z < 0) continue;

                    float pixelDepth = bary.x * vertsCamera[tri.ver_i[0]].z +
                        bary.y * vertsCamera[tri.ver_i[1]].z +
                        bary.z * vertsCamera[tri.ver_i[2]].z;
                    pixelDepth = -pixelDepth;

                    if (pixelDepth < 0) continue;
                    if (pixelDepth < im.getDepth(x, y))
                    {
                        // interpolate normal
                        Vec3f normal = (norms[tri.nor_i[0]] * bary.x +
                            norms[tri.nor_i[1]] * bary.y +
                            norms[tri.nor_i[2]] * bary.z).normalize();
                        
                        // interpolate position
                        Vec3f position = verts[tri.ver_i[0]] * bary.x +
                            verts[tri.ver_i[1]] * bary.y +
                            verts[tri.ver_i[2]] * bary.z;

                        // interpolate texture coord
                        Vec2f texCoord = mesh.textures[tri.tex_i[0]] * bary.x +
                            mesh.textures[tri.tex_i[1]] * bary.y +
                            mesh.textures[tri.tex_i[2]] * bary.z;

                        // get texture color
                        const Image& texDiffuse = model.material.diffuse;
                        Vec2i imCoordDiffuse = Vec2i(
                            (int) (texCoord.x * (texDiffuse.width-1)),
                            (int) ((1.f-texCoord.y) * (texDiffuse.height-1))
                        );

                        // add normal map
                        const Image& texNormal = model.material.normal;
                        Vec2i imCoordNormal = Vec2i(
                            (int) (texCoord.x * (texNormal.width-1)),
                            (int) ((1.f-texCoord.y) * (texNormal.height-1))
                        );
                        const Color& normalColor = texNormal.getPixel(imCoordNormal.x, imCoordNormal.y);
                        Vec3f mapNormal(
                            (normalColor.r - 128) / 128.f,
                            (normalColor.g - 128) / 128.f,
                            (normalColor.b - 128) / 128.f
                        );
                        normal = (normal + mapNormal).normalize();

                        // get specular map
                        const Image& texSpecular = model.material.specular;
                        Vec2i imCoordSpecular = Vec2i(
                            (int) (texCoord.x * (texSpecular.width-1)),
                            (int) ((1.f-texCoord.y) * (texSpecular.height-1))
                        );
                        float specularColor = texSpecular.getPixel(imCoordSpecular.x, imCoordSpecular.y).r / 255.f;

                        const Color& texColor = texDiffuse.getPixel(imCoordDiffuse.x, imCoordDiffuse.y);
                        Color finalColor(0, 0, 0);
                        finalColor += texColor * ambientLight;

                        // get light intensity
                        for (Light* light : lights)
                        {
                            float lightAmount = light->getLighting(position, normal);
                            finalColor += texColor * light->getColor() * pow(lightAmount, specularColor * 32.f + 1.f);
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
        delete[] vertsCamera;
        delete[] norms;
    }

    void RenderLights(Image& im, const Camera& cam, const std::vector<Light*>& lights)
    {
        Quaternion antiCamRot = cam.rotation.conjugate();
        float lightRadius = 4.f;

        for (Light* light : lights)
        {
            Vec3f cameraPoint = antiCamRot * (light->position - cam.position);
            Vec2i p = ProjectPoint(im.width, im.height, cameraPoint);

            DisplayCircle(im, p, lightRadius, light->color);
            DisplayLine(
                im,
                p,
                ProjectPoint(im.width, im.height, antiCamRot * ((light->position + light->forward()/2) - cam.position)),
                light->color
            );
        }
    }

    void RenderScene(Image& im, const Camera& cam, const Scene& scene)
    {
        const std::vector<Light*>& lights = scene.getLights();
        for (Model* model : scene.getModels())
            RenderModel(im, cam, *model, scene.getAmbientLight(), lights);
        RenderLights(im, cam, lights);
    }
}
