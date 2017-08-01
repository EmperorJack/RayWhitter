//
// Created by purvisjack on 1/08/17.
//

#ifndef RAYWHITTER_SCENE_HPP
#define RAYWHITTER_SCENE_HPP

#include <vector>
#include <primatives.hpp>
#include <intersection.hpp>
#include <plane.hpp>
#include <sphere.hpp>
#include <point_light.hpp>

class Scene {

public:
    Scene(glm::vec3 cameraPosition);
    Intersection intersect(Ray ray);

    glm::vec3 cameraPosition;
    std::vector<Shape*> shapes;
    std::vector<PointLight*> lights;

};


#endif //RAYWHITTER_SCENE_HPP
