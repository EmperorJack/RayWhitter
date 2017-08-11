//
// Created by Jack Purvis
//

#ifndef RAYWHITTER_MATERIAL_HPP
#define RAYWHITTER_MATERIAL_HPP

#include <glm/glm.hpp>
#include <core/intersection.hpp>

class Material {

public:
    Material(float kd = 0.0f, float ks = 0.0f, float n = 8.0f, float kr = 0.0f, float kt = 0.0f, float ior = 1.0f) :
            kd(kd), ks(ks), n(n), kr(kr), kt(kt), ior(ior) {}
    virtual glm::vec3 evaluate(Ray ray, Intersection intersect, glm::vec3 lightDirection, glm::vec3 lightIntensity, float a);

    // Matte
    float kd;

    // Phong
    float ks;
    float n;

    // Reflection
    float kr;

    // Refraction
    float kt;
    float ior;

};

#endif //RAYWHITTER_MATERIAL_HPP
