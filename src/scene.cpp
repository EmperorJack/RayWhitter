//
// Created by purvisjack on 1/08/17.
//

#include "scene.hpp"

Scene::Scene(glm::vec3 cameraPosition,  std::vector<Plane> planes, std::vector<Sphere> spheres, std::vector<PointLight> lights) {
    this->cameraPosition = cameraPosition;
    this->planes = planes;
    this->spheres = spheres;
    this->lights = lights;
}

bool Scene::intersect(Ray ray) {
    for (Sphere sphere : spheres) {
        if (sphere.intersect(ray)) return true;
    }

    return false;
}
