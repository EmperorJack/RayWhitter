//
// Created by Purvis
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <shapes/mesh.hpp>
#include <core/material.hpp>

bool Mesh::intersect(Ray ray, float &t, glm::vec3 &n) {
    bool hit = false;
    t = 10000000000.0f;
    glm::vec3 nNear;

    for (Sphere* sphere : spheres) {
        glm::vec3 sn;
        float st = 10000000000.0f;
        if (sphere->intersect(ray, st, sn) && st < t) {
            hit = true;
            t = st;
            nNear = sn;
        }
    }

    return hit;
}

void Mesh::loadOBJFile(std::string filename) {

    // Make sure our geometry information is cleared
    points.clear();
    uvs.clear();
    normals.clear();
    triangles.clear();

    // Load dummy points because OBJ indexing starts at 1 not 0
    points.push_back(glm::vec3(0, 0, 0));
    uvs.push_back(glm::vec2(0, 0));
    normals.push_back(glm::vec3(0, 0, 1));

    // Attempt to open an input stream to the file
    std::ifstream objFile(filename);
    if (!objFile.is_open()) {
        std::cout << "Error reading " << filename << std::endl;
        return;
    }

    // While the file has lines remaining
    while (objFile.good()) {

        // Pull out line from file
        std::string line;
        std::getline(objFile, line);
        std::istringstream objLine(line);

        // Pull out mode from line
        std::string mode;
        objLine >> mode;

        // Reading like this means whitespace at the start of the line is fine
        // attempting to read from an empty string/line will set the failbit
        if (!objLine.fail()) {

            if (mode == "v") {
                // Parse vertex
                glm::vec3 v;
                objLine >> v.x >> v.y >> v.z;
                points.push_back(v);
            } else if (mode == "vn") {
                // Parse vertex normal
                glm::vec3 vn;
                objLine >> vn.x >> vn.y >> vn.z;
                normals.push_back(vn);
            } else if (mode == "vt") {
                // Parse UV
                glm::vec2 vt;
                objLine >> vt.x >> vt.y;
                uvs.push_back(vt);
            } else if (mode == "f") {
                // Parse face
                std::vector<vertex> verts;
                while (objLine.good()) {
                    vertex v;

                    objLine >> v.p; // Scan in position index

                    // Check if the obj file contains normal data
                    if (normals.size() > 1) {
                        // obj face is formatted as v/vt/vn or v//vn
                        objLine.ignore(1); // Ignore the '/' character

                        if (uvs.size() > 1) { // Check if the obj file contains uv data
                            objLine >> v.t; // Scan in uv (texture coord) index
                        }
                        objLine.ignore(1); // Ignore the '/' character

                        objLine >> v.n; // Scan in normal index
                    } // Else the obj face is formatted just as v

                    verts.push_back(v);
                }

                // If we have 3 verticies, construct a triangle
                if (verts.size() >= 3) {
                    triangle tri;
                    tri.v[0] = verts[0];
                    tri.v[1] = verts[1];
                    tri.v[2] = verts[2];
                    triangles.push_back(tri);
                }
            }
        }
    }

    generateSurfaceNormals();

    // Test spheres for each vertex
    Material* phong = new Material(0.8f, 0.2f, 12);
    for (glm::vec3 point : points) {
        spheres.push_back(new Sphere(position + point * 10.0f, glm::vec3(1, 0, 0), phong, 0.25f));
    }
}

void Mesh::generateSurfaceNormals() {
    for (int i = 0; i < triangles.size(); i++) {
        glm::vec3 surfaceNormal = glm::cross(points[triangles[i].v[1].p] - points[triangles[i].v[0].p],
                                   points[triangles[i].v[2].p] - points[triangles[i].v[0].p]);
        surfaceNormals.push_back(glm::normalize(surfaceNormal));
    }
}
