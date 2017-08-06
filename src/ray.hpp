//
// Created by Jack Purvis on 5/08/17.
//

#ifndef RAYWHITTER_RAY_HPP
#define RAYWHITTER_RAY_HPP

#include <glm/glm.hpp>

class Ray {

public:
    Ray(int depth) :
        depth(depth) {}
    glm::vec3 position(float t);

    glm::vec3 origin;
    glm::vec3 direction;
    int depth;

};

#endif //RAYWHITTER_RAY_HPP
