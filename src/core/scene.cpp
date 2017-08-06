//
// Created by Jack Purvis
//

#include <core/scene.hpp>

Intersection Scene::intersect(Ray ray) {
    Intersection intersection;
    intersection.hit = false;
    intersection.t = infinity;
    intersection.shape = nullptr;

    for (Shape* shape : shapes) {
        float t = infinity;
        glm::vec3 normal;
        if (shape->intersect(ray, t, normal) && t < intersection.t) {
            intersection.hit = true;
            intersection.t = t;
            intersection.shape = shape;
            intersection.normal = normal;
        }
    }

    return intersection;
}
