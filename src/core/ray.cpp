//
// Created by Jack Purvis on 5/08/17.
//

#include <core/ray.hpp>

glm::vec3 Ray::position(float t) {
    return origin + t * direction;
}
