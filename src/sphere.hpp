//
// Created by purvisjack on 1/08/17.
//

#ifndef RAYWHITTER_SPHERE_HPP
#define RAYWHITTER_SPHERE_HPP

#include <primatives.hpp>

class Sphere {

public:
    Sphere(glm::vec3 position, float radius);
    bool intersect(Ray ray);

private:
    glm::vec3 position;
    float radius;

};


#endif //RAYWHITTER_SPHERE_HPP
