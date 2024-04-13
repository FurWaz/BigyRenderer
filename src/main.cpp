#include <iostream>
#include <signal.h>
#include <string>
#include <chrono>
#include <thread>
#include <math.h>
#include "Image.hpp"
#include "Renderer.hpp"
#include "ModelLoader.hpp"
#include "SceneLoader.hpp"

bool loop = true;

void intHandler(int sig)
{
    loop = false;
}

int main(int argc, char const *argv[])
{
    signal(SIGINT, intHandler);

    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <path_model>" << std::endl;
        return 1;
    }

    std::string path_scene = argv[1];
    std::cout << "Loading scene from [" << path_scene << "] ..." << std::endl;
    Scene scene = SceneLoader::FromFile(path_scene);
    if (scene.empty())
    {
        std::cerr << "Error : loaded scene is empty !" << std::endl;
        exit(1);
    }
    std::cout << "Scene loaded successfully !" << std::endl;
    
    Image image(500, 800);
    Camera camera(Vec3f(0, 0.3f, 1.5f), Quaternion::Euler(-0.2f, 0, 0));
    float radius = 2.f;

    float rot = 0;
    while (loop)
    {
        rot += 0.1f;
        camera.position = Vec3f(
            cos(rot) * radius,
            0.2f + sin(1.3f * rot) * 0.5f,
            sin(rot) * radius
        );
        camera.lookAt(Vec3f(0.f, 0.2f, 0.f));

        image.clear(Color::BLACK);
        Renderer::RenderScene(image, camera, scene);
        image.save("./output.png");
        image.saveDepth("./depth.png");
    }

    return 0;
}
