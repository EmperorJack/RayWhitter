//
// Created by Jack Purvis
//

#ifndef RAYWHITTER_SCENE_HPP
#define RAYWHITTER_SCENE_HPP

#include <vector>
#include <core/ray.hpp>
#include <core/intersection.hpp>
#include <lights/point_light.hpp>

class Scene {

public:
    Scene(glm::vec3 cameraPosition) : cameraPosition(cameraPosition) {}
    Intersection intersect(Ray ray);

    glm::vec3 cameraPosition;
    std::vector<Shape*> shapes;
    std::vector<PointLight*> lights;

};


#endif //RAYWHITTER_SCENE_HPP
