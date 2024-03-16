#include <iostream>
#include <signal.h>
#include <string>
#include <chrono>
#include <math.h>
#include "Image.hpp"
#include "Renderer.hpp"
#include "ModelLoader.hpp"
#include "Vec3.hpp"
#include "Camera.hpp"

void intHandler(int sig)
{
    exit(0);
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
    
    Image image("./output.png");
    image.load();
    Camera camera(Vec3f(0, 0, 0), Quaternion::Euler(0, 0, 0));
    Renderer::RenderModel(image, camera, model, Color(64, 128, 255, 64));
    image.save("./output.png");

    return 0;
}
