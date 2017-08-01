//
// Created by Playtech on 2/08/2017.
//

#ifndef RAYWHITTER_INTERSECTION_HPP
#define RAYWHITTER_INTERSECTION_HPP

#include <plane.hpp>
#include <sphere.hpp>

class Intersection {

public:
    bool hit;
    Sphere* sphere;
    Plane* plane;

};

#endif //RAYWHITTER_INTERSECTION_H
