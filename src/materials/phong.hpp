//
// Created by Jack Purvis
//

#ifndef RAYWHITTER_PHONG_HPP
#define RAYWHITTER_PHONG_HPP

#include <materials/material.hpp>

class Phong : public Material {

public:
    Phong(float kd, float ks, float n) :
            kd(kd), ks(ks), n(n) {}
    glm::vec3 evaluate(Ray ray, Intersection intersect, glm::vec3 lightDirection, glm::vec3 lightIntensity, float a);

    float kd;
    float ks;
    float n;

};

#endif //RAYWHITTER_PHONG_HPP
