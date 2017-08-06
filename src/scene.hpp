//
// Created by purvisjack on 1/08/17.
//

#ifndef RAYWHITTER_SCENE_HPP
#define RAYWHITTER_SCENE_HPP

#include <vector>
#include <ray.hpp>
#include <intersection.hpp>
#include <plane.hpp>
#include <sphere.hpp>
#include <point_light.hpp>

const float infinity = 10000000000.0f;

class Scene {

public:
    Scene(glm::vec3 cameraPosition) : cameraPosition(cameraPosition) {}
    Intersection intersect(Ray ray);

    glm::vec3 cameraPosition;
    std::vector<Shape*> shapes;
    std::vector<PointLight*> lights;

};


#endif //RAYWHITTER_SCENE_HPP
