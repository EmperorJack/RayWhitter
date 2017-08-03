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
    intersection.t = 10000000000;
    intersection.shape = nullptr;

    for (Shape* shape : shapes) {
        float t = 10000000000;
        if (shape->intersect(ray, t) && t < intersection.t) {
            intersection.t = t;
            intersection.hit = true;
            intersection.t = t;
            intersection.shape = shape;
        }
    }

    return intersection;
}
