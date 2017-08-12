//
// Created by Jack Purvis
//

#ifndef RAYWHITTER_MESH_HPP
#define RAYWHITTER_MESH_HPP

#include <string>
#include <vector>
#include <shapes/shape.hpp>
#include <shapes/sphere.hpp>
#include <core/bounding_box.hpp>

struct vertex {
    int p;
    int t;
    int n;
};

struct triangle {
    vertex v[3];
};

class Mesh : public Shape {

public:
    Mesh(glm::vec3 position, glm::vec3 albedo, Material* material, std::string filename, float scale = 1.0f) :
    Shape(position, albedo, material), scale(scale) { loadOBJFile(filename); }
    bool intersect(Ray ray, float &t, glm::vec3 &n);

private:
    void loadOBJFile(std::string filename);
    void generateSurfaceNormals();
    bool rayTriangleIntersect(Ray ray, float &t, int triangleIndex, float &u, float &v);

    std::vector<glm::vec3> points;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
    std::vector<triangle> triangles;
    std::vector<glm::vec3> surfaceNormals;
    float scale;
    BoundingBox* boundingBox;

};

#endif //RAYWHITTER_MESH_HPP
