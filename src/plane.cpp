//
// Created by purvisjack on 1/08/17.
//

#include <plane.hpp>

bool Plane::intersect(Ray ray, float &t, glm::vec3 &n) {
    float num = glm::dot(position - ray.origin, normal);
    float denom = glm::dot(normal, ray.direction);

    // Ray is parallel with plane
    if (fabs(denom) < 0.0001f) return false;

    t = num / denom;

    if (t >= 0.0001f) {
        n = normal;
        return true;
    }

    return false;
}
