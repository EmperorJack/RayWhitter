//
// Created by Jack Purvis
//

#ifndef RAYWHITTER_INTERSECTION_HPP
#define RAYWHITTER_INTERSECTION_HPP

#include <shapes/shape.hpp>

class Intersection {

public:
    bool hit;
    float t;
    Shape* shape;
    glm::vec3 normal;

};

#endif //RAYWHITTER_INTERSECTION_H
