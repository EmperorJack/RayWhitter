//
// Created by Jack Purvis
//

#ifndef RAYWHITTER_SHAPE_HPP
#define RAYWHITTER_SHAPE_HPP

#include <glm/glm.hpp>
#include <core/ray.hpp>

static int nextId = 0;

class Material;

class Shape {

public:
    Shape(glm::vec3 position, glm::vec3 albedo, Material* material) :
            position(position), albedo(albedo), material(material), id(nextId++) {}
    virtual bool intersect(Ray ray, float &t, glm::vec3 &n, bool useBoundingBox);

    glm::vec3 position;
    glm::vec3 albedo;
    Material* material;

    int id;

};

#endif //RAYWHITTER_SHAPE_HPP
