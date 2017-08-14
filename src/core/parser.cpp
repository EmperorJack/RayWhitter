//
// Created by Jack Purvis
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <core/renderer.hpp>
#include <core/material.hpp>
#include <shapes/plane.hpp>
#include <shapes/sphere.hpp>
#include <shapes/mesh.hpp>

using std::string;

struct parseObject {
    string className;
    std::map<string, std::vector<string>> attributes;
};

std::vector<string> split(string str) {
    string buffer;
    std::stringstream stringStream(str);

    std::vector<string> tokens;

    while (stringStream >> buffer) {
        tokens.push_back(buffer);
    }

    return tokens;
}

std::vector<parseObject> getParseObjects(std::ifstream &fileStream) {
    string line;
    std::vector<string> lineTokens;

    // Setup parse objects
    std::vector<parseObject> parsedObjects;

    // Parse each object declared in the scene file
    while (std::getline(fileStream, line)) {
        lineTokens = split(line);

        if (lineTokens.size() == 0) continue;

        // Check for a "Begin"
        if (split(line)[0].compare("Begin") == 0) {

            parseObject parseObj;
            parseObj.className = lineTokens[1];

            // Process attributes until an "End" is hit
            while (std::getline(fileStream, line)) {
                lineTokens = split(line);
                if (lineTokens[0].compare("End") == 0) break;

                // Get the attribute name
                string attribute = lineTokens[0];

                // Get the attribute values
                std::vector<string> values;
                for (int i = 1; i < lineTokens.size(); i++) {
                    values.push_back(lineTokens[i]);
                }
                parseObj.attributes[attribute] = values;
            }

            parsedObjects.push_back(parseObj);
        }
    }

    return parsedObjects;
}

glm::vec3 parseVector(std::vector<string> values) {
    return glm::vec3(
            std::stof(values[0]),
            std::stof(values[1]),
            std::stof(values[2])
    );
}

Material* parseMaterial(parseObject parseObj) {
    Material* material = new Material();

    if (parseObj.attributes.count("kd") != 0)
        material->kd = std::stof(parseObj.attributes["kd"][0]);
    if (parseObj.attributes.count("ks") != 0)
        material->ks = std::stof(parseObj.attributes["ks"][0]);
    if (parseObj.attributes.count("n") != 0)
        material->n = std::stof(parseObj.attributes["n"][0]);
    if (parseObj.attributes.count("kr") != 0)
        material->kr = std::stof(parseObj.attributes["kr"][0]);
    if (parseObj.attributes.count("kt") != 0)
        material->kt = std::stof(parseObj.attributes["kt"][0]);
    if (parseObj.attributes.count("ior") != 0)
        material->ior = std::stof(parseObj.attributes["ior"][0]);

    return material;
}

Plane* parsePlane(parseObject parseObj, std::map<string, Material*> materials) {
    glm::vec3 position, albedo, normal;
    Material* material = nullptr;

    if (parseObj.attributes.count("position") != 0)
        position = parseVector(parseObj.attributes["position"]);
    if (parseObj.attributes.count("albedo") != 0)
        albedo = parseVector(parseObj.attributes["albedo"]);
    if (parseObj.attributes.count("material") != 0)
        material = materials[parseObj.attributes["material"][0]];
    if (parseObj.attributes.count("normal") != 0)
        normal = parseVector(parseObj.attributes["normal"]);

    // Set the default material if none as given
    if (material == nullptr) {
        material = materials["default"];
    }

    return new Plane(position, albedo, material, normal);
}

Sphere* parseSphere(parseObject parseObj, std::map<string, Material*> materials) {
    glm::vec3 position, albedo;
    float radius = 1.0f;
    Material* material = nullptr;

    if (parseObj.attributes.count("position") != 0)
        position = parseVector(parseObj.attributes["position"]);
    if (parseObj.attributes.count("albedo") != 0)
        albedo = parseVector(parseObj.attributes["albedo"]);
    if (parseObj.attributes.count("material") != 0)
        material = materials[parseObj.attributes["material"][0]];
    if (parseObj.attributes.count("radius") != 0)
        radius = std::stof(parseObj.attributes["radius"][0]);

    // Set the default material if none as given
    if (material == nullptr) {
        material = materials["default"];
    }

    return new Sphere(position, albedo, material, radius);
}

Mesh* parseMesh(parseObject parseObj, std::map<string, Material*> materials) {
    glm::vec3 position, albedo;
    string filename = "";
    float scale = 1.0f;
    Material* material = nullptr;

    if (parseObj.attributes.count("position") != 0)
        position = parseVector(parseObj.attributes["position"]);
    if (parseObj.attributes.count("albedo") != 0)
        albedo = parseVector(parseObj.attributes["albedo"]);
    if (parseObj.attributes.count("material") != 0)
        material = materials[parseObj.attributes["material"][0]];
    if (parseObj.attributes.count("filename") != 0)
        filename = parseObj.attributes["filename"][0];
    if (parseObj.attributes.count("scale") != 0)
        scale = std::stof(parseObj.attributes["scale"][0]);

    // Set the default material if none as given
    if (material == nullptr) {
        material = materials["default"];
    }

    return new Mesh(position, albedo, material, filename, scale);
}

PointLight* parsePointLight(parseObject parseObj) {
    glm::vec3 position, colour;
    float intensity = 1.0f;

    if (parseObj.attributes.count("position") != 0)
        position = parseVector(parseObj.attributes["position"]);
    if (parseObj.attributes.count("intensity") != 0)
        intensity = std::stoi(parseObj.attributes["intensity"][0]);
    if (parseObj.attributes.count("colour") != 0)
        colour = parseVector(parseObj.attributes["colour"]);

    return new PointLight(position, intensity, colour);
}

void parseSceneFile(char* sceneFilename, Renderer &renderer, Scene &scene, int &imageWidth, int &imageHeight) {
    std::ifstream fileStream(sceneFilename);
    string line;
    std::vector<string> lineTokens;

    // Expect the first line to be the image resolution
    std::getline(fileStream, line);
    lineTokens = split(line);
    imageWidth = std::stoi(lineTokens[0]);
    imageHeight = std::stoi(lineTokens[1]);

    // Get the parse objects from the remainder of the file
    std::vector<parseObject> parseObjects = getParseObjects(fileStream);

    // Store a list of materials including a default material
    std::map<string, Material*> materials;
    materials["default"] = new Material(0.5f);

    // Construct the scene and renderer objects from the parse objects
    for (parseObject parseObj : parseObjects) {
        if (parseObj.className.compare("Renderer") == 0) {
            if (parseObj.attributes.count("fov") != 0)
                renderer.fov = std::stoi(parseObj.attributes["fov"][0]);
            if (parseObj.attributes.count("maxBounces") != 0)
                renderer.maxBounces = std::stoi(parseObj.attributes["maxBounces"][0]);
            if (parseObj.attributes.count("antiAliasing") != 0)
                renderer.antiAliasingAmount = std::stoi(parseObj.attributes["antiAliasing"][0]);
            if (parseObj.attributes.count("useBoundingBox") != 0)
                renderer.useBoundingBox = (bool) std::stoi(parseObj.attributes["useBoundingBox"][0]);
        } else if (parseObj.className.compare("Camera") == 0) {
            if (parseObj.attributes.count("position") != 0)
                scene.cameraPosition = parseVector(parseObj.attributes["position"]);
        } else if (parseObj.className.compare("Material") == 0) {
            if (parseObj.attributes.count("name") != 0)
                materials[parseObj.attributes["name"][0]] = parseMaterial(parseObj);
        } else if (parseObj.className.compare("Plane") == 0) {
            scene.shapes.push_back(parsePlane(parseObj, materials));
        } else if (parseObj.className.compare("Sphere") == 0) {
            scene.shapes.push_back(parseSphere(parseObj, materials));
        } else if (parseObj.className.compare("Mesh") == 0) {
            scene.shapes.push_back(parseMesh(parseObj, materials));
        } else if (parseObj.className.compare("PointLight") == 0) {
            scene.lights.push_back(parsePointLight(parseObj));
        }
    }
}
