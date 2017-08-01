//
// Created by purvisjack on 1/08/17.
//

#ifndef RAYWHITTER_PRIMATIVES_HPP
#define RAYWHITTER_PRIMATIVES_HPP

#include <glm/glm.hpp>

struct Ray {
    glm::vec3 origin;
    glm::vec3 direction;
    float time;

    glm::vec3 getPosition() {
        return origin + direction * time;
    }
};

#endif //RAYWHITTER_PRIMATIVES_HPP
