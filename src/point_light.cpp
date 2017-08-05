//
// Created by purvisjack on 1/08/17.
//

#include "point_light.hpp"

PointLight::PointLight(glm::vec3 position, float intensity, glm::vec3 colour) {
    this->position = position;
    this->intensity = intensity;
    this->colour = colour;
}

void PointLight::illuminate(glm::vec3 point, glm::vec3 &lightDirection, glm::vec3 &lightIntensity, float &distance) {
    lightDirection = point - position;
    distance = glm::length(lightDirection);
    // distance = sqrtf(distanceSquared);
    lightDirection = glm::normalize(lightDirection);
    // lightDirection.x /= distance, lightDirection.y /= distance, lightDirection.z /= distance;
    // avoid division by 0
    lightIntensity = (colour * intensity); // / (4 * (float) M_PI * distance * distance);

}
