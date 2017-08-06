//
// Created by Jack Purvis
//

#include <materials/matte.hpp>

glm::vec3 Matte::evaluate(Ray ray, Intersection intersect, glm::vec3 lightDirection, glm::vec3 lightIntensity, float a) {
    glm::vec3 diffuse = lightIntensity * intersect.shape->albedo * a;

    return diffuse * kd;
}
