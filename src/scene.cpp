//
// Created by purvisjack on 1/08/17.
//

#include <scene.hpp>

Scene::Scene(glm::vec3 cameraPosition) {
    this->cameraPosition = cameraPosition;
}

Intersection Scene::intersect(Ray ray) {
    Intersection intersection;
    intersection.hit = false;
    intersection.t = infinity;
    intersection.shape = nullptr;

    for (Shape* shape : shapes) {
        float t = infinity;
        if (shape->intersect(ray, t) && t < intersection.t) {
            intersection.hit = true;
            intersection.t = t;
            intersection.shape = shape;
        }
    }

    return intersection;
}
