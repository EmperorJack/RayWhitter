//
// Created by purvisjack on 1/08/17.
//

#include <sphere.hpp>
#include <algorithm>
#include <iostream>

Sphere::Sphere(glm::vec3 position, float radius) {
    this->position = position;
    this->radius = radius;
}

bool Sphere::intersect(Ray ray, float &t) {
    glm::vec3 l = position - ray.origin;
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

        if (t0 < 0.0f && t1 < 0.0f) {
//            std::cout << "both behind" << std::endl;
            return false;
        }
        if (t0 < 0.0f && t1 > 0.0f) {
//            std::cout << "one in front" << std::endl;
            t = t1;
            return true;
        }
        if (t1 < 0.0f && t0 > 0.0f) {
//            std::cout << "one in front" << std::endl;
            t = t0;
            return true;
        }

//        std::cout << "two in front" << std::endl;

        t = std::min(t0, t1);
        return true;
    }

    return false;
}

glm::vec3 Sphere::getNormal(Ray ray) {
    return glm::normalize(ray.getPosition() - position);
}

glm::vec3 Sphere::getColour() {
    return glm::vec3(0.0f, 0.0f, 1.0f);
}
