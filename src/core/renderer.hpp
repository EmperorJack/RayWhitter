//
// Created by Jack Purvis
//

#ifndef RAYWHITTER_RENDERER_HPP
#define RAYWHITTER_RENDERER_HPP

#include <glm/glm.hpp>
#include <core/scene.hpp>

class Renderer {

public:
    glm::vec3** render(int width, int height);
    glm::vec3 castRay(Ray ray, Scene scene);

private:
    Scene makeScene();

    glm::vec3 backgroundColour = glm::vec3(0.8f);
    glm::vec3 shadowColour = glm::vec3(0);

    float fov = 90.0f;
    const int maxBounces = 10;
    const bool distributed = false;

};

#endif //RAYWHITTER_RENDERER_HPP
