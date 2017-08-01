//
// Created by purvisjack on 1/08/17.
//

#include <sphere.hpp>

Sphere::Sphere(glm::vec3 position, float radius) {
    this->position = position;
    this->radius = radius;
}

bool Sphere::intersect(Ray ray, float &t) {
    t = glm::distance(position, ray.getPosition());
    return t < radius;
}

glm::vec3 Sphere::getColour() {
    return glm::vec3(0.0f, 0.0f, 1.0f);
}
