//
// Created by Jack Purvis
//

#ifndef RAYWHITTER_REFRACTIVE_HPP
#define RAYWHITTER_REFRACTIVE_HPP

#include <materials/reflective.hpp>

class Refractive : public Reflective {

public:
    Refractive(float kd, float ks, float n, float kr, float kt, float index) :
            Reflective(kd, ks, n, kr), kt(kt), ior(index) {}
    glm::vec3 evaluateRefraction(Renderer* renderer, Scene scene, Ray ray, Intersection intersect);

    float kt;
    float ior;

};

#endif //RAYWHITTER_REFRACTIVE_HPP
