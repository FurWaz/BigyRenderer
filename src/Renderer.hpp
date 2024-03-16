#pragma once
#include "Camera.hpp"
#include "Image.hpp"
#include "Model.hpp"
#include "Vec2.hpp"
#include "Draw.hpp"

namespace Renderer
{
    Vec2i CameraToScreen(const Image &im, const Vec3f& point)
    {
        return Vec2i(
            (int) ((point.x + 1) * (float)(im.width / 2)),
            im.height - (int) ((point.y + 1) * (float)(im.height / 2))
        );
    }

    Vec3f WorldToCamera(const Camera& cam, const Vec3f& world)
    {
        return cam.rotation.conjugate() * (world - cam.position);
    }

    void RenderTriangle(Image &im, const Camera& cam, Triangle t, const Color &color)
    {
        Vec3f p1 = t.vertices[t.vertices_index[0]];
        Vec3f p2 = t.vertices[t.vertices_index[1]];
        Vec3f p3 = t.vertices[t.vertices_index[2]];

        Vec2i p_1 = CameraToScreen(im, WorldToCamera(cam, p1));
        Vec2i p_2 = CameraToScreen(im, WorldToCamera(cam, p2));
        Vec2i p_3 = CameraToScreen(im, WorldToCamera(cam, p3));

        Draw::Line(im, p_1, p_2, color);
        Draw::Line(im, p_2, p_3, color);
        Draw::Line(im, p_3, p_1, color);
    }

    void RenderModel(Image& im, const Camera& cam, const Model& model, const Color &color = Color::WHITE)
    {
        for (size_t i = 0; i < model.n_triangles; i++)
            RenderTriangle(im, cam, model.triangles[i], color);
    }
}
