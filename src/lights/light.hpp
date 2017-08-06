//
// Created by Jack Purvis
//

#ifndef RAYWHITTER_LIGHT_HPP
#define RAYWHITTER_LIGHT_HPP

#include <glm/glm.hpp>

class Light {

public:
    Light(float intensity, glm::vec3 colour) : intensity(intensity), colour(colour) {}
    virtual void illuminate(glm::vec3 point, glm::vec3 &lightDirection, glm::vec3 &lightIntensity, float &distance);

    float intensity;
    glm::vec3 colour;

};

#endif //RAYWHITTER_LIGHT_HPP
