//
// Created by purvisjack on 1/08/17.
//

#include <plane.hpp>

Plane::Plane(glm::vec3 position, glm::vec3 normal, float width, float height) {
    this->position = position;
    this->normal = normal;
    this->width = width;
    this->height = height;
}

bool Plane::intersect(Ray ray) {
    float num = glm::dot(position - ray.origin, normal);
    float denom = glm::dot(normal, ray.direction);

    // Ray is parallel with plane
    if (fabs(denom) < 0.0001f) return false;

    float t = num / denom;

    return t >= 0.0001f;
}
