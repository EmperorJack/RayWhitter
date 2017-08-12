//
// Created by Jack Purvis
//

#ifndef RAYWHITTER_MESH_HPP
#define RAYWHITTER_MESH_HPP

#include <string>
#include <vector>
#include <shapes/shape.hpp>
#include <shapes/sphere.hpp>

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
    Mesh(glm::vec3 position, glm::vec3 albedo, Material* material, std::string filename) :
    Shape(position, albedo, material), triangles(triangles) { loadOBJFile(filename); }
    bool intersect(Ray ray, float &t, glm::vec3 &n);

private:
    void loadOBJFile(std::string filename);
    void generateSurfaceNormals();

    std::vector<glm::vec3> points;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
    std::vector<triangle> triangles;
    std::vector<glm::vec3> surfaceNormals;

    std::vector<Sphere*> spheres;

};

#endif //RAYWHITTER_MESH_HPP
