//
// Created by purvisjack on 1/08/17.
//

#include "point_light.hpp"

PointLight::PointLight(glm::vec3 position, glm::vec3 intensity) {
    this->position = position;
    this->intensity = intensity;
}