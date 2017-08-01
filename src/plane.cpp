//
// Created by purvisjack on 1/08/17.
//

#include "plane.hpp"

Plane::Plane(glm::vec3 position, float width, float height) {
    this->position = position;
    this->width = width;
    this->height = height;
}

bool Plane::intersect(Ray ray) {
    return false;
}