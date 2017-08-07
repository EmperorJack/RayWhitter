//
// Created by Jack Purvis
//

#include <materials/reflective.hpp>

glm::vec3 Reflective::evaluateReflection(Renderer* renderer, Scene scene, Ray ray, Intersection intersect) {
    if (kr <= 0.0f) return glm::vec3();

    glm::vec3 reflect = glm::reflect(ray.direction, intersect.normal);

    Ray reflectRay = Ray(ray.depth + 1);
    reflectRay.direction = reflect;
    reflectRay.origin = intersect.point + 1.0f * reflectRay.direction;

    return kr * renderer->castRay(reflectRay, scene);
}
