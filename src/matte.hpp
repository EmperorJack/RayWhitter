//
// Created by Playtech on 6/08/2017.
//

#ifndef RAYWHITTER_MATTE_HPP
#define RAYWHITTER_MATTE_HPP

#include <material.hpp>

class Matte : public Material {

public:
    Matte(float kd) :
            kd(kd) {}
    glm::vec3 evaluate(Ray ray, Intersection intersect, glm::vec3 lightDirection, glm::vec3 lightIntensity, float a);

    float kd;

};

#endif //RAYWHITTER_MATTE_HPP
