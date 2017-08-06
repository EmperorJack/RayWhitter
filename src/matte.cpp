//
// Created by Playtech on 6/08/2017.
//

#include <matte.hpp>

glm::vec3 Matte::evaluate(Ray ray, Intersection intersect, glm::vec3 lightDirection, glm::vec3 lightIntensity, float a) {
    glm::vec3 diffuse = lightIntensity * intersect.shape->albedo * a;

    return diffuse * kd;
}
