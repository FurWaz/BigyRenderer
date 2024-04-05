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
#include "Light.hpp"

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

    std::string path_model = argv[1];
    Model model = ModelLoader::FromFolder(path_model);
    if (!model.valid())
    {
        std::cerr << "Error: invalid model!" << std::endl;
        return 1;
    }
    
    Image image(500, 800);
    Camera camera(Vec3f(0, 0.3f, 1.5f), Quaternion::Euler(-0.2f, 0, 0));

    std::vector<Light*> lights;
    Light* whiteLight = new Light(Vec3f(0, 0, 0), Color::WHITE, 2, 2.0f);
    Light* redLight = new Light(Vec3f(0, 0, 0), Color::RED, 1, 0.5f);
    Light* greenLight = new Light(Vec3f(0, 0, 0), Color::GREEN, 1, 0.5f);
    lights.push_back(whiteLight);
    lights.push_back(redLight);
    lights.push_back(greenLight);

    float rot = 0;
    while (loop)
    {
        rot += 0.1f;
        float whiteRadius = 0.7f;
        float redRadius = 0.2f;
        whiteLight->position = Vec3f(cos(rot) * whiteRadius, 0,  sin(rot) * whiteRadius);
        redLight->position = Vec3f(  cos(rot*1.3f) * redRadius,  sin(rot*1.3f) * redRadius, 0.1f);
        greenLight->position = Vec3f(-cos(rot*1.3f) * redRadius, -sin(rot*1.3f) * redRadius, 0.1f);
        model.mesh.rotation = Quaternion::Euler(0, rot * 0.7f, 0);

        image.clear(Color::BLACK);
        Renderer::RenderLights(image, camera, lights);
        Renderer::RenderModel(image, camera, model, lights);
        image.save("./output.png");
        // image.saveDepth("./depth.png");
    }

    return 0;
}
