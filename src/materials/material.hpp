//
// Created by Jack Purvis
//

#ifndef RAYWHITTER_MATERIAL_HPP
#define RAYWHITTER_MATERIAL_HPP

#include <glm/glm.hpp>
#include <core/renderer.hpp>

class Material {

public:
    virtual glm::vec3 evaluate(Ray ray, Intersection intersect, glm::vec3 lightDirection, glm::vec3 lightIntensity, float a);
    virtual glm::vec3 evaluateReflection(Renderer* renderer, Scene scene, Ray ray, Intersection intersect);
    virtual glm::vec3 evaluateRefraction(Renderer* renderer, Scene scene, Ray ray, Intersection intersect);

};

#endif //RAYWHITTER_MATERIAL_HPP
