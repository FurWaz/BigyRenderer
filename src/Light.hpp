#pragma once
#include "Vec3.hpp"
#include "Color.hpp"
#include "Object.hpp"
#include "Utils.hpp"

class Light : public Object
{
protected:
    Light();
    Light(Vec3f pos, Quaternion rot, Color c, float intensity);

    virtual float getIntensity(const Vec3f& point, const Vec3f& normal) const = 0;

public:
    Color color;
    float intensity;
    float* shadowMap;
    int shadowMapSize;
    
    virtual ~Light();

    void bakeShadows(const std::vector<Model*>& models, int shadowMapSize = 256);
    float getLighting(const Vec3f& point, const Vec3f& normal) const;
    virtual Vec2i projectPoint(const Vec3f& point) const = 0;
    const Color& getColor() const;
};

Light::Light()
    : Object(), color(Color::WHITE), intensity(1.0)
{
    shadowMap = nullptr;
    shadowMapSize = 0;
}

Light::Light(Vec3f pos, Quaternion rot, Color c, float intensity)
    : Object(pos, rot), color(c), intensity(intensity)
{
    shadowMap = nullptr;
    shadowMapSize = 0;
}

Light::~Light()
{
    if (shadowMap != nullptr)
        delete[] shadowMap;
}

void Light::bakeShadows(const std::vector<Model*>& models, int shadowMapSize)
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
            projected[i] = this->projectPoint(verts[i]);
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

            for (int x = b1.x; x < b2.x; x++)
            {
                for (int y = b1.y; y < b2.y; y++)
                {
                    Vec2i p(x, y);
                    if (x < 0 || x >= shadowMapSize || y < 0 || y >= shadowMapSize)
                        continue;
                    
                    Vec3f bary = Utils::barycentric(p1, p2, p3, p);
                    if (bary.x < 0 || bary.y < 0 || bary.z < 0) continue;

                    Vec3f local = verts[tri.ver_i[0]] * bary.x +
                        verts[tri.ver_i[1]] * bary.y +
                        verts[tri.ver_i[2]] * bary.z;
                    float pixelDepth = local.length();

                    if (pixelDepth < 0) continue;
                    if (pixelDepth < shadowMap[x + y * shadowMapSize])
                        shadowMap[x + y * shadowMapSize] = pixelDepth;
                }
            }
        }
    }
}

float Light::getLighting(const Vec3f& point, const Vec3f& normal) const
{
    if (shadowMap != nullptr)
    {
        Vec3f local = rotation.conjugate() * (point - position);
        Vec2i coords = this->projectPoint(local);
        if (coords.x < 0 || coords.x >= shadowMapSize ||
            coords.y < 0 || coords.y >= shadowMapSize)
            return 0.f; // not in light view, so in the shadows

        float shadowDepth = shadowMap[coords.x + coords.y * shadowMapSize];
        float pointDepth = local.length();
        float bias = 0.1f; // margin to avoid stairs effect for horizontal surfaces
        if (pointDepth > shadowDepth + bias) return 0.f; // in the shadows (behind triangle)
    }
    return this->getIntensity(point, normal);
}

const Color& Light::getColor() const
{
    return color;
}
