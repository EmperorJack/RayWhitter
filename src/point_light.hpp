//
// Created by purvisjack on 1/08/17.
//

#ifndef RAYWHITTER_POINT_LIGHT_HPP
#define RAYWHITTER_POINT_LIGHT_HPP

#include <glm/glm.hpp>

class PointLight {

public:
    PointLight(glm::vec3 position, float intensity, glm::vec3 colour) :
        position(position), intensity(intensity), colour(colour) {}
    void illuminate(glm::vec3 point, glm::vec3 &lightDirection, glm::vec3 &lightIntensity, float &distance);

private:
    glm::vec3 position;
    float intensity;
    glm::vec3 colour;

};

#endif //RAYWHITTER_POINT_LIGHT_HPP
