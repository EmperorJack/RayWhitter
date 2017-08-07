//
// Created by Jack Purvis
//

#include <iostream>
#include <materials/refractive.hpp>

double clamp(double upper, double lower, double x) {
    return std::min(upper, std::max(x, lower));
}

glm::vec3 Refractive::evaluateRefraction(Renderer* renderer, Scene scene, Ray ray, Intersection intersect) {
    if (kt <= 0.0f) return glm::vec3();

    float cosRayDirection = clamp(-1, 1, glm::dot(ray.direction, intersect.normal));

    float iorLeaving = 1;
    float iorEntering = ior;

    glm::vec3 normal = glm::vec3(intersect.normal);

    // Entering the medium
    if (cosRayDirection < 0) {
        cosRayDirection = -cosRayDirection;
    }

    // Leaving the medium
    else {
        std::swap(iorLeaving, iorEntering);
        normal = -normal;
    }

    float iorRatio = iorLeaving / iorEntering;

    float k = 1 - iorRatio * iorRatio * (1 - cosRayDirection * cosRayDirection);

    glm::vec3 refractDirection = k < 0 ? glm::vec3() :
                                 iorRatio * ray.direction + (iorRatio * cosRayDirection - sqrtf(k)) * normal;

    Ray refractRay = Ray(ray.depth + 1);
    refractRay.direction = refractDirection;
    refractRay.origin = intersect.point + 1.0f * refractRay.direction;

    return kt * renderer->castRay(refractRay, scene);
}
