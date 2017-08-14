//
// Created by Jack Purvis
//

#ifndef RAYWHITTER_RENDERER_HPP
#define RAYWHITTER_RENDERER_HPP

#include <glm/glm.hpp>
#include <core/scene.hpp>

class Renderer {

public:
    glm::vec3** render(int width, int height, Scene scene);

    float fov = 90.0f;
    int maxBounces = 5;
    int antiAliasingAmount = 1; // 1 is no anti-aliasing
    bool useBoundingBox = true;

private:
    glm::vec3 castRay(Scene scene, Ray ray);
    glm::vec3 evaluatePhong(Ray ray, Intersection intersect, glm::vec3 lightDirection, glm::vec3 lightIntensity, float a);
    glm::vec3 evaluateReflection(Scene scene, Ray ray, Intersection intersect);
    glm::vec3 evaluateRefraction(Scene scene, Ray ray, Intersection intersect);
    void fresnel(Ray ray, Intersection intersect, float ior, float &kr);

    glm::vec3 backgroundColour = glm::vec3(0.8f);
    glm::vec3 shadowColour = glm::vec3(0);

};

#endif //RAYWHITTER_RENDERER_HPP
