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
    Intersection intersect(Ray ray);

    glm::vec3 cameraPosition = glm::vec3(0, 0, 0);
    std::vector<Shape*> shapes;
    std::vector<PointLight*> lights;

};

#endif //RAYWHITTER_SCENE_HPP
