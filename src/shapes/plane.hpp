//
// Created by Jack Purvis
//

#ifndef RAYWHITTER_PLANE_HPP
#define RAYWHITTER_PLANE_HPP

#include <shapes/shape.hpp>

class Plane : public Shape {

public:
    Plane(glm::vec3 position, glm::vec3 albedo, Material* material, glm::vec3 normal):
            Shape(position, albedo, material), normal(normal) {}
    bool intersect(Ray ray, float &t, glm::vec3 &n, bool useBoundingBox);

private:
    glm::vec3 normal;

};

#endif //RAYWHITTER_PLANE_HPP
