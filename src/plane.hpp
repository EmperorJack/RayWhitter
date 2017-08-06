//
// Created by purvisjack on 1/08/17.
//

#ifndef RAYWHITTER_PLANE_HPP
#define RAYWHITTER_PLANE_HPP

#include <ray.hpp>
#include <shape.hpp>

class Plane : public Shape {

public:
    Plane(glm::vec3 position, glm::vec3 albedo, Material material, glm::vec3 normal):
            Shape(position, albedo, material), normal(normal) {}
    bool intersect(Ray ray, float &t);
    glm::vec3 getNormal(glm::vec3 point);
    glm::vec3 getColour();

private:
    glm::vec3 normal;

};

#endif //RAYWHITTER_PLANE_HPP
