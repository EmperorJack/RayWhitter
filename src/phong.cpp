//
// Created by Playtech on 6/08/2017.
//

#include <algorithm>
#include <phong.hpp>

glm::vec3 Phong::evaluate(Ray ray, Intersection intersect, glm::vec3 lightDirection, glm::vec3 lightIntensity, float a) {
    glm::vec3 reflect = glm::reflect(lightDirection, intersect.normal);

    glm::vec3 diffuse = lightIntensity * intersect.shape->albedo * a;
    glm::vec3 specular = lightIntensity * std::pow(std::max(0.0f, glm::dot(reflect, -ray.direction)), n);

    return diffuse * kd + specular * ks;
}