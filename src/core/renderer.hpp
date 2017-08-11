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

private:
    Scene makeScene();
    glm::vec3 castRay(Scene scene, Ray ray);
    glm::vec3 evaluateReflection(Scene scene, Ray ray, Intersection intersect, float kr);
    glm::vec3 evaluateRefraction(Scene scene, Ray ray, Intersection intersect, float kt, float ior);

    glm::vec3 backgroundColour = glm::vec3(0.8f);
    glm::vec3 shadowColour = glm::vec3(0);

    float fov = 90.0f;
    const int maxBounces = 4;
    const bool distributed = false;

};

#endif //RAYWHITTER_RENDERER_HPP
