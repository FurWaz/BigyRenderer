#include <iostream>
#include <signal.h>
#include <string>
#include <chrono>
#include <thread>
#include <math.h>
#include "Image.hpp"
#include "Renderer.hpp"
#include "ModelLoader.hpp"
#include "Vec3.hpp"
#include "Camera.hpp"

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
        std::cerr << "Usage: " << argv[0] << " <path_obj>" << std::endl;
        return 1;
    }

    std::string path_obj = argv[1];
    Timer timerLoad;
    Model model = ModelLoader::FromFile(path_obj);
    timerLoad.print("Load time: ");
    if (!model.valid())
    {
        std::cerr << "Error: invalid model!" << std::endl;
        return 1;
    }
    
    Image image(512, 512);
    Camera camera(Vec3f(0, 0, 1.5), Quaternion::Euler(0, 0, 0));

    float rot = 0;
    while (loop)
    {
        rot += 0.1f;
        model.rotation = Quaternion::Euler(0, rot, 0);

        image.clear(Color::BLACK);
        Renderer::RenderModel(image, camera, model);
        image.save("./output.png");
        image.saveDepth("./depth.png");
    }

    return 0;
}
