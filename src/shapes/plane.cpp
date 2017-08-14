//
// Created by Jack Purvis
//

#include <shapes/plane.hpp>

bool Plane::intersect(Ray ray, float &t, glm::vec3 &n, bool useBoundingBox) {
    float num = glm::dot(position - ray.origin, normal);
    float denom = glm::dot(normal, ray.direction);

    // Ray is parallel with plane
    if (fabs(denom) < 0.000001f) return false;

    t = num / denom;

    if (t >= 0.000001f) {
        n = normal;
        return true;
    }

    return false;
}
