//
// Created by purvisjack on 1/08/17.
//

#ifndef RAYWHITTER_PRIMATIVES_HPP
#define RAYWHITTER_PRIMATIVES_HPP

#include <glm/glm.hpp>

const float infinity = 10000000000.0f;

struct Ray {
    glm::vec3 origin;
    glm::vec3 direction;
};

#endif //RAYWHITTER_PRIMATIVES_HPP
