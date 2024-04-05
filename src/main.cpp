#include <iostream>
#include <signal.h>
#include <string>
#include <chrono>
#include <thread>
#include <math.h>
#include "Image.hpp"
#include "Renderer.hpp"
#include "MeshLoader.hpp"
#include "Vec3.hpp"
#include "Camera.hpp"
#include "Light.hpp"

bool loop = true;

void intHandler(int sig)
{
    loop = false;
}

int main(int argc, char const *argv[])
{
    signal(SIGINT, intHandler);

    if (argc < 3)
    {
        std::cerr << "Usage: " << argv[0] << " <path_obj> <path_tex>" << std::endl;
        return 1;
    }

    std::string path_obj = argv[1];
    Mesh mesh = MeshLoader::FromFile(path_obj);
    if (!mesh.valid())
    {
        std::cerr << "Error: invalid model!" << std::endl;
        return 1;
    }

    std::string path_tex = argv[2];
    Image im(path_tex);
    im.load();
    Material mat;
    mat.texture = im;
    mesh.material = mat;
    
    Image image(768, 1024);
    Camera camera(Vec3f(0, 0, 1.5), Quaternion::Euler(0, 0, 0));

    std::vector<Light*> lights;
    lights.push_back(new Light(Vec3f(1, 0, 0), Color::RED, 1, 1.0f));
    lights.push_back(new Light(Vec3f(-1, 0, 0), Color::GREEN, 1, 1.0f));
    lights.push_back(new Light(Vec3f(0, 2, 1.5), Color::BLUE, 1, 1.0f));

    float rot = 0;
    while (loop)
    {
        rot += 0.1f;
        mesh.rotation = Quaternion::Euler(0, rot, 0);

        image.clear(Color::BLACK);
        Renderer::RenderMesh(image, camera, mesh, lights);
        image.save("./output.png");
        image.saveDepth("./depth.png");
    }

    return 0;
}
