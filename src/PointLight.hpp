#pragma once
#include "Light.hpp"
#include "Model.hpp"

class PointLight : public Light
{
protected:
    float range;

public:
    PointLight(Vec3f pos, Quaternion rot, Color col, float intensity, float range)
        : Light(pos, rot, col, intensity), range(range)
    {

    }

    void bakeShadows(const std::vector<Model*>& models, int shadowMapSize = 256)
    {
        if (shadowMap != nullptr) delete[] shadowMap;
        this->shadowMap = new float[shadowMapSize * shadowMapSize];
        this->shadowMapSize = shadowMapSize;

        for (int i = 0; i < shadowMapSize * shadowMapSize; i++)
            this->shadowMap[i] = std::numeric_limits<float>::infinity();
            
        Quaternion antiRot = rotation.conjugate();
        for (Model* model : models)
        {
            const Mesh& mesh = model->mesh;
            Vec3f* verts = new Vec3f[mesh.n_vertices];
            Vec2i* projected = new Vec2i[mesh.n_vertices];

            for (int i = 0; i < mesh.n_vertices; i++)
            {
                verts[i] = (mesh.rotation * mesh.vertices[i]) + mesh.position;
                verts[i] = antiRot * (verts[i] - position);
                // TODO : change Renderer::ProjectPoint to own overriden method
                //        to permit point, spot, and direction lights (360deg, for example)
                projected[i] = Renderer::ProjectPoint(shadowMapSize, shadowMapSize, verts[i]);
            }

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

                bool outOfImage = (b1.x < 0 || b1.x >= shadowMapSize ||
                                  b1.y < 0 || b1.y >= shadowMapSize) &&
                                  (b2.x < 0 || b2.x >= shadowMapSize ||
                                  b2.y < 0 || b2.y >= shadowMapSize);
                if (outOfImage) continue;

                for (int x = b1.x; x < b2.x; x++)
                {
                    for (int y = b1.y; y < b2.y; y++)
                    {
                        Vec2i p(x, y);
                        if (x < 0 || x >= shadowMapSize || y < 0 || y >= shadowMapSize)
                            continue;
                        
                        Vec3f bary = Renderer::barycentric(p1, p2, p3, p);
                        if (bary.x < 0 || bary.y < 0 || bary.z < 0) continue;

                        float pixelDepth = bary.x * verts[tri.ver_i[0]].z +
                            bary.y * verts[tri.ver_i[1]].z +
                            bary.z * verts[tri.ver_i[2]].z;
                        pixelDepth = -pixelDepth;

                        if (pixelDepth < 0) continue;
                        if (pixelDepth < shadowMap[x + y * shadowMapSize])
                            shadowMap[x + y * shadowMapSize] = pixelDepth;
                    }
                }
            }
        }
    }

    float getIntensity(Vec3f point, Vec3f normal) const
    {
        if (shadowMap != nullptr)
        {
            Vec3f local = rotation.conjugate() * (point - position);
            Vec2i coords = Renderer::ProjectPoint(shadowMapSize, shadowMapSize, local);
            if (coords.x < 0 || coords.x >= shadowMapSize ||
                coords.y < 0 || coords.y >= shadowMapSize)
                return 0.f; // not in light cone

            float shadowDepth = shadowMap[coords.x + coords.y * shadowMapSize];
            float bias = 0.1f; // margin to avoid stairs effect for horizontal surfaces
            if (-local.z > shadowDepth + bias) return 0.f; // in the shadows (behind triangle)
        }

        Vec3f shift = position - point;
        Vec3f lightDir = shift.normalize();
        float distance = shift.length();
        float angle = lightDir.dot(normal);
        if (angle < 0.f) angle = 0.f;
        float range = 1.f - (distance / this->range);
        if (range < 0.f) range = 0.f;
        if (range > 1.f) range = 1.f;
        return angle * intensity;
    }
};
