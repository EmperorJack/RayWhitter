//
// Created by Jack Purvis
//

#ifndef RAYWHITTER_REFLECTIVE_HPP
#define RAYWHITTER_REFLECTIVE_HPP

#include <materials/phong.hpp>

class Reflective : public Phong {

public:
    Reflective(float kd, float ks, float n, float kr) :
            Phong(kd, ks, n), kr(kr) {}
    glm::vec3 evaluateReflection(Renderer* renderer, Scene scene, Ray ray, Intersection intersect);

    float kr;

};

#endif //RAYWHITTER_REFLECTIVE_HPP
