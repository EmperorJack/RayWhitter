//
// Created by Jack Purvis
//

#include <materials/material.hpp>

glm::vec3 Material::evaluate(Ray ray, Intersection intersect, glm::vec3 lightDirection, glm::vec3 lightIntensity, float a) {
    return glm::vec3();
}

glm::vec3 Material::evaluateReflection(Renderer* renderer, Scene scene, Ray ray, Intersection intersect) {
    return glm::vec3();
}

glm::vec3 Material::evaluateRefraction(Renderer* renderer, Scene scene, Ray ray, Intersection intersect) {
    return glm::vec3();
}
