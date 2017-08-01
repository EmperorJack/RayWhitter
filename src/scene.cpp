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
    intersection.plane = nullptr;
    intersection.sphere = nullptr;

    for (Plane* plane : planes) {
        if (plane->intersect(ray)) {
            intersection.hit = true;
            intersection.plane = plane;
            return intersection;
        }
    }

    for (Sphere* sphere : spheres) {
        if (sphere->intersect(ray)) {
            intersection.hit = true;
            intersection.sphere = sphere;
            return intersection;
        }
    }

    return intersection;
}
