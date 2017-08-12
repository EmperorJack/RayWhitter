//
// Created by purvisjack on 13/08/17.
//

#include <core/bounding_box.hpp>
#include <core/constants.hpp>
#include <iostream>
#include <algorithm>

BoundingBox::BoundingBox(std::vector<glm::vec3> points) {
    xMin = infinity;
    xMax = -infinity;
    yMin = infinity;
    yMax = -infinity;
    zMin = infinity;
    zMax = -infinity;

    // Compute the box that bounds the mesh
    for (int i = 1; i < points.size(); i++) {
        glm::vec3 point = points[i];

        if (point.x < xMin) xMin = point.x;
        if (point.x > xMax) xMax = point.x;

        if (point.y < yMin) yMin = point.y;
        if (point.y > yMax) yMax = point.y;

        if (point.z < zMin) zMin = point.z;
        if (point.z > zMax) zMax = point.z;
    }
}

bool BoundingBox::intersect(Ray ray) {

    // Compute x slab intersection interval
    float xtNear = (xMin - ray.origin.x) / ray.direction.x;
    float xtFar = (xMax - ray.origin.x) / ray.direction.x;

    if (xtNear > xtFar) std::swap(xtNear, xtFar);

    // Compute y slab intersection interval
    float ytNear = (yMin - ray.origin.y) / ray.direction.y;
    float ytFar = (yMax - ray.origin.y) / ray.direction.y;

    if (ytNear > ytFar) std::swap(ytNear, ytFar);

    // Compute z slab intersection interval
    float ztNear = (zMin - ray.origin.z) / ray.direction.z;
    float ztFar = (zMax - ray.origin.z) / ray.direction.z;

    if (ztNear > ztFar) std::swap(ztNear, ztFar);

    // Check x and y overlap
    bool xyOverlap = xtNear <= ytFar && ytNear <= xtFar;

    // Check x and z overlap
    bool xzOverlap = xtNear <= ztFar && ztNear <= xtFar;

    // Check y and z overlap
    bool yzOverlap = ytNear <= ztFar && ztNear <= ytFar;

    return xyOverlap && xzOverlap && yzOverlap;
}
