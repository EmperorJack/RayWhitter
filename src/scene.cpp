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
    intersection.shape = nullptr;

    float tNear = 10000000000;

    for (Shape* shape : shapes) {
        float t = 10000000000;
        if (shape->intersect(ray, t) && t < tNear) {
            tNear = t;
            intersection.hit = true;
            intersection.shape = shape;
        }
    }

    return intersection;
}
