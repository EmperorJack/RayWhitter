//
// Created by Jack Purvis
//

#ifndef RAYWHITTER_SPHERE_HPP
#define RAYWHITTER_SPHERE_HPP

#include <shapes/shape.hpp>

class Sphere : public Shape {

public:
    Sphere(glm::vec3 position, glm::vec3 albedo, Material* material, float radius) :
            Shape(position, albedo, material), radius(radius) {}
    bool intersect(Ray ray, float &t, glm::vec3 &n, bool useBoundingBox);

private:
    float radius;

};

#endif //RAYWHITTER_SPHERE_HPP
