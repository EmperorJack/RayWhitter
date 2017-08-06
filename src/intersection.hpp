//
// Created by Playtech on 2/08/2017.
//

#ifndef RAYWHITTER_INTERSECTION_HPP
#define RAYWHITTER_INTERSECTION_HPP

#include <shape.hpp>

class Intersection {

public:
    bool hit;
    float t;
    Shape* shape;
    glm::vec3 normal;

};

#endif //RAYWHITTER_INTERSECTION_H
