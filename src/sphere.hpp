//
// Created by purvisjack on 1/08/17.
//

#ifndef RAYWHITTER_SPHERE_HPP
#define RAYWHITTER_SPHERE_HPP

#include <primatives.hpp>
#include <shape.hpp>

class Sphere : public Shape {

public:
    Sphere(glm::vec3 position, float radius, int id);
    bool intersect(Ray ray, float &t);
    glm::vec3 getNormal(glm::vec3 point);
    glm::vec3 getColour();

private:
    glm::vec3 position;
    float radius;

};

#endif //RAYWHITTER_SPHERE_HPP
