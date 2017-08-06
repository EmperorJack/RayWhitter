//
// Created by purvisjack on 1/08/17.
//

#include <plane.hpp>

bool Plane::intersect(Ray ray, float &t) {
    float num = glm::dot(position - ray.origin, normal);
    float denom = glm::dot(normal, ray.direction);

    // Ray is parallel with plane
    if (fabs(denom) < 0.0001f) return false;

    t = num / denom;

    return t >= 0.0001f;
}

glm::vec3 Plane::getNormal(glm::vec3 point) {
    return normal;
}

glm::vec3 Plane::getColour() {
    return glm::vec3(1.0f, 0.0f, 0.0f);
}
