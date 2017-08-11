//
// Created by Jack Purvis
//

#include <algorithm>
#include <materials/material.hpp>

glm::vec3 Material::evaluate(Ray ray, Intersection intersect, glm::vec3 lightDirection, glm::vec3 lightIntensity, float a) {
    glm::vec3 diffuse = lightIntensity * intersect.shape->albedo * a;

    glm::vec3 specular;
    if (ks > 0.0f) {
        glm::vec3 reflect = glm::reflect(lightDirection, intersect.normal);
        glm::vec3 specular = lightIntensity * std::pow(std::max(0.0f, glm::dot(reflect, -ray.direction)), n);
    }

    return diffuse * kd + specular * ks;
}
