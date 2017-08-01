//
// Created by purvisjack on 1/08/17.
//

#include "sphere.hpp"

Sphere::Sphere(glm::vec3 position, float radius) {
    this->position = position;
    this->radius = radius;
}

bool Sphere::intersect(Ray ray) {
    return false;
}