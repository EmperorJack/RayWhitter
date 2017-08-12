//
// Created by Jack Purvis
//

#ifndef RAYWHITTER_BOUNDING_BOX_HPP
#define RAYWHITTER_BOUNDING_BOX_HPP

#include <vector>
#include <glm/glm.hpp>
#include <core/ray.hpp>

class BoundingBox {

public:
    BoundingBox(std::vector<glm::vec3> points);
    bool intersect(Ray ray);

private:
    float xMin, xMax;
    float yMin, yMax;
    float zMin, zMax;

};

#endif //RAYWHITTER_BOUNDING_BOX_HPP
