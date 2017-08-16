//
// Created by Jack Purvis
//

#define _USE_MATH_DEFINES
#include <cmath>
#include <lights/point_light.hpp>

void PointLight::illuminate(glm::vec3 point, glm::vec3 &lightDirection, glm::vec3 &lightIntensity, float &distance) {
    lightDirection = point - position;
    distance = glm::length(lightDirection);
    lightDirection = glm::normalize(lightDirection);
    lightIntensity = (colour * intensity) / (float) (4 * M_PI * std::pow(distance, 2));
}
