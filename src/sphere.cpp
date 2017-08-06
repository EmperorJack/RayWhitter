//
// Created by purvisjack on 1/08/17.
//

#include <iostream>
#include <algorithm>
#include <sphere.hpp>

bool Sphere::intersect(Ray ray, float &t, glm::vec3 &n) {
    glm::vec3 l = ray.origin - position;
    float a = glm::dot(ray.direction, ray.direction);
    float b = 2.0f * glm::dot(ray.direction, l);
    float c = glm::dot(l, l) - (radius * radius);

    float det = (b * b) - (4 * a * c);

    if (det < 0.0f) return false;

    if (det == 0.0f) {
        t = -b / (2 * a);
        return t >= 0.0001f;
    }

    if (det > 0.0f) {
        float t0 = (-b + sqrtf(det)) / (2 * a);
        float t1 = (-b - sqrtf(det)) / (2 * a);

        // Both behind
        if (t0 < 0.0f && t1 < 0.0f) return false;

        // One in front
        else if (t0 < 0.0f && t1 > 0.0f) t = t1;

        // Other one in front
        else if (t1 < 0.0f && t0 > 0.0f) t = t0;

        // Two in front
        else t = std::min(t0, t1);

        n = glm::normalize(ray.position(t) - position);

        return true;
    }

    return false;
}
