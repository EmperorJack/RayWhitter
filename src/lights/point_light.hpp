//
// Created by Jack Purvis
//

#ifndef RAYWHITTER_POINT_LIGHT_HPP
#define RAYWHITTER_POINT_LIGHT_HPP

#include <glm/glm.hpp>
#include <lights/light.hpp>

class PointLight : public Light {

public:
    PointLight(glm::vec3 position, float intensity, glm::vec3 colour) :
        Light(intensity, colour), position(position) {}
    void illuminate(glm::vec3 point, glm::vec3 &lightDirection, glm::vec3 &lightIntensity, float &distance);

private:
    glm::vec3 position;

};

#endif //RAYWHITTER_POINT_LIGHT_HPP
