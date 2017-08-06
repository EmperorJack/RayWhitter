//
// Created by Jack Purvis
//

#ifndef RAYWHITTER_MATERIAL_HPP
#define RAYWHITTER_MATERIAL_HPP

#include <glm/glm.hpp>
#include <core/intersection.hpp>

class Material {

public:
    Material(float kr) :
            kr(kr) {}
    virtual glm::vec3 evaluate(Ray ray, Intersection intersect, glm::vec3 lightDirection, glm::vec3 lightIntensity, float a);

    float kr;

};

#endif //RAYWHITTER_MATERIAL_HPP
