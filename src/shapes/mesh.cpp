//
// Created by Purvis
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <shapes/mesh.hpp>
#include <core/constants.hpp>

bool Mesh::intersect(Ray ray, float &t, glm::vec3 &n, bool useBoundingBox) {

    // Ensure the ray intersects the bounding box before testing each triangle
    if (useBoundingBox && !boundingBox->intersect(ray)) return false;

    bool hit = false;
    t = infinity;
    int closestIndex = 0;
    glm::vec2 uv;

    for (int triangleIndex = 0; triangleIndex < triangles.size(); triangleIndex++) {
        float tTri = infinity;
        float u, v;
        if (rayTriangleIntersect(ray, tTri, triangleIndex, u, v) && tTri < t) {
            hit = true;
            t = tTri;
            closestIndex = triangleIndex;
            uv.x = u;
            uv.y = v;
        }
    }

    if (hit) {
        // Compute a smooth interpolated normal
        glm::vec3 n0 = normals[triangles[closestIndex].v[0].n];
        glm::vec3 n1 = normals[triangles[closestIndex].v[1].n];
        glm::vec3 n2 = normals[triangles[closestIndex].v[2].n];
        n = glm::normalize((1 - uv.x - uv.y) * n0 + uv.x * n1 + uv.y * n2);
    }

    return hit;
}

bool Mesh::rayTriangleIntersect(Ray ray, float &t, int triangleIndex, float &u, float &v) {

    // Get the triangle properties
    glm::vec3 v0 = points[triangles[triangleIndex].v[0].p];
    glm::vec3 v1 = points[triangles[triangleIndex].v[1].p];
    glm::vec3 v2 = points[triangles[triangleIndex].v[2].p];
    glm::vec3 n = surfaceNormals[triangleIndex];

    glm::vec3 v0v1 = v1 - v0;
    glm::vec3 v0v2 = v2 - v0;
    glm::vec3 pvec = glm::cross(ray.direction, v0v2);
    float det = glm::dot(v0v1, pvec);

    // Check if ray and triangle are parallel
    if (fabs(det) < 0.0001f) return false;

    float invDet = 1.0f / det;

    glm::vec3 tvec = ray.origin - v0;
    u = glm::dot(tvec, pvec) * invDet;
    if (u < 0 || u > 1) return false;

    glm::vec3 qvec = glm::cross(tvec, v0v1);
    v = glm::dot(ray.direction, qvec) * invDet;
    if (v < 0 || u + v > 1) return false;

    t = glm::dot(v0v2, qvec) * invDet;

    return t > 0;
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
                v = (v * scale) + position;
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

    boundingBox = new BoundingBox(points);
}

void Mesh::generateSurfaceNormals() {
    for (int i = 0; i < triangles.size(); i++) {
        glm::vec3 surfaceNormal = glm::cross(points[triangles[i].v[1].p] - points[triangles[i].v[0].p],
                                   points[triangles[i].v[2].p] - points[triangles[i].v[0].p]);
        surfaceNormals.push_back(glm::normalize(surfaceNormal));
    }
}
