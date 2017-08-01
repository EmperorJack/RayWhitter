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
    glm::vec3 rayPos = ray.getPosition();

    return (position.x - width / 2 < rayPos.x && rayPos.x < position.x + width / 2 &&
            position.y - height / 2 < rayPos.y && rayPos.y < position.y + height / 2 &&
            rayPos.z > position.z);
}
