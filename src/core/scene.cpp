//
// Created by Jack Purvis
//

#include <glm/glm.hpp>
#include <core/scene.hpp>
#include <core/constants.hpp>

Intersection Scene::intersect(Ray ray, bool useBoundingBox) {
    Intersection intersection;
    intersection.hit = false;
    intersection.t = infinity;
    intersection.shape = nullptr;

    for (Shape* shape : shapes) {
        float t = infinity;
        glm::vec3 normal;
        if (shape->intersect(ray, t, normal, useBoundingBox) && t < intersection.t) {
            intersection.hit = true;
            intersection.t = t;
            intersection.shape = shape;
            intersection.normal = normal;
        }
    }

    if (intersection.shape != nullptr) intersection.point = ray.position(intersection.t);

    return intersection;
}
