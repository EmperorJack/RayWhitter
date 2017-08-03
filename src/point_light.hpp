//
// Created by purvisjack on 1/08/17.
//

#ifndef RAYWHITTER_POINT_LIGHT_HPP
#define RAYWHITTER_POINT_LIGHT_HPP

#include <glm/glm.hpp>

class PointLight {

public:
    PointLight(glm::vec3 position, glm::vec3 intensity);

//private:
    glm::vec3 position;
    glm::vec3 intensity;
};

#endif //RAYWHITTER_POINT_LIGHT_HPP
