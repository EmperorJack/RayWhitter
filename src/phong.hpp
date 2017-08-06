//
// Created by Playtech on 6/08/2017.
//

#ifndef RAYWHITTER_PHONG_HPP
#define RAYWHITTER_PHONG_HPP

#include <material.hpp>

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
