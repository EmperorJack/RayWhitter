//
// Created by Playtech on 6/08/2017.
//

#ifndef RAYWHITTER_MATERIAL_HPP
#define RAYWHITTER_MATERIAL_HPP

#include <glm/glm.hpp>
#include <intersection.hpp>

class Material {

public:
    virtual glm::vec3 evaluate(Ray ray, Intersection intersect, glm::vec3 lightDirection, glm::vec3 lightIntensity, float a);

};

#endif //RAYWHITTER_MATERIAL_HPP
