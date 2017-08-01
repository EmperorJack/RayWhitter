//
// Created by purvisjack on 1/08/17.
//

#ifndef RAYWHITTER_PLANE_HPP
#define RAYWHITTER_PLANE_HPP

#include <primatives.hpp>

class Plane {

public:
    Plane(glm::vec3 position, glm::vec3 normal, float width, float height);
    bool intersect(Ray ray);

private:
    glm::vec3 position;
    glm::vec3 normal;
    float width;
    float height;

};

#endif //RAYWHITTER_PLANE_HPP
