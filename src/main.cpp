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
        std::cerr << "Usage: " << argv[0] << " <path_scene>" << std::endl;
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
    
    Image image(350, 600);
    Camera camera(Vec3f(0.3f, 0.3f, 1.5f), Quaternion::Euler(0, 0, 0));
    float radius = 2.f;

    std::cout << "Baking lights ..." << std::endl;
    const std::vector<Light*>& lights = scene.getLights();
    for (int i = 0; i < lights.size(); i++)
    {
        lights[i]->bakeShadows(scene.getModels(), 512);
        Image::saveDepth(std::string("./shadowmap_") + std::to_string(i) + ".png", lights[i]->shadowMapSize, lights[i]->shadowMapSize, lights[i]->shadowMap);
    }
    std::cout << "Done baking !" << std::endl;

    std::cout << "Rendering ..." << std::endl;
    float rot = 0.f;
    std::chrono::time_point<std::chrono::system_clock> start, end;
    while (loop)
    {
        start = std::chrono::system_clock::now();
        rot += 0.03f;
        camera.position = Vec3f(
            cos(rot) * radius,
            0.2f + sin(1.3f * rot) * 0.5f,
            sin(rot) * radius
        );
        camera.lookAt(Vec3f(0.f, 0.2f, 0.f));

        image.clear(scene.getAmbientLight());
        Renderer::RenderScene(image, camera, scene);
        image.save("./output.png");
        end = std::chrono::system_clock::now();

        std::chrono::duration<double> elapsed_seconds = end - start;

        std::cout << "\rFPS : " << (int)(1 / elapsed_seconds.count()) << "   ";
    }

    std::cout << std::endl;
    std::cout << "Exiting ..." << std::endl;
    return 0;
}
