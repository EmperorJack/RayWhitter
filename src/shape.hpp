//
// Created by Playtech on 2/08/2017.
//

#ifndef RAYWHITTER_SHAPE_HPP
#define RAYWHITTER_SHAPE_HPP

#include <glm/glm.hpp>
#include <primatives.hpp>

class Shape {

public:
    virtual bool intersect(Ray ray, float &t);
    virtual glm::vec3 getNormal(Ray ray);
    virtual glm::vec3 getColour();

};

#endif //RAYWHITTER_SHAPE_HPP
