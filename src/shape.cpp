//
// Created by Playtech on 2/08/2017.
//

#include <shape.hpp>

bool Shape::intersect(Ray ray, float &t) {
    return true;
}

glm::vec3 Shape::getNormal(Ray ray) {
    return glm::vec3();
}

glm::vec3 Shape::getColour() {
    return glm::vec3(0, 0, 0);
}
