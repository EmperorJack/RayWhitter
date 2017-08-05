//
// Created by Playtech on 2/08/2017.
//

#ifndef RAYWHITTER_SHAPE_HPP
#define RAYWHITTER_SHAPE_HPP

#include <glm/glm.hpp>
#include <ray.hpp>

class Shape {

public:
    virtual bool intersect(Ray ray, float &t);
    virtual glm::vec3 getNormal(glm::vec3 point);
    virtual glm::vec3 getColour();

    int id;
};

#endif //RAYWHITTER_SHAPE_HPP
