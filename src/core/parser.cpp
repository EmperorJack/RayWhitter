//
// Created by Jack Purvis
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <core/scene.hpp>
#include <core/material.hpp>
#include <shapes/plane.hpp>
#include <shapes/sphere.hpp>
#include <shapes/mesh.hpp>

using std::string;

void println(string content) {
    std::cout << content.c_str() << std::endl;
}

void print(string content) {
    std::cout << content.c_str();
}

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
        if (split(line)[0].compare(string("Begin")) == 0) {

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

Scene parseSceneFile(char* sceneFilename, int &imageWidth, int &imageHeight) {
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

    Scene scene = Scene(glm::vec3(0.0f, 0.0f, 0.0f));
    return scene;
}

//
//Scene makeScene() {
//    glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 0.0f);
//
//    Scene scene = Scene(cameraPosition);
//
//    Material* matte = new Material(1);
//    Material* phong = new Material(0.8f, 0.2f, 12);
//    Material* mirror = new Material(0, 0.2f, 8, 0.8f);
//    Material* mirrorGround = new Material(0.7f, 0, 0, 0.3f);
//    Material* glass = new Material(0, 0, 0, 0, 1, 1.5f);
//    Material* refract = new Material(0, 0, 0, 1.0f, 1.0f, 1.7f);
//
//    // Room
//    scene.shapes.push_back(new Plane(glm::vec3(0.0f, -120.0f, 0.0f), glm::vec3(1, 1, 1), matte, glm::normalize(glm::vec3(0, 1, 0))));
//    scene.shapes.push_back(new Plane(glm::vec3(0.0f, 120.0f, 0.0f), glm::vec3(1, 1, 1), matte, glm::normalize(glm::vec3(0, -1, 0))));
//    scene.shapes.push_back(new Plane(glm::vec3(200.0f, 0.0f, 0.0f), glm::vec3(1, 0, 0), matte, glm::normalize(glm::vec3(-1, 0, 0))));
//    scene.shapes.push_back(new Plane(glm::vec3(-200.0f, 0.0f, 0.0f), glm::vec3(0, 1, 0), matte, glm::normalize(glm::vec3(1, 0, 0))));
//    scene.shapes.push_back(new Plane(glm::vec3(0.0f, 0.0f, -200.0f), glm::vec3(0, 0, 1), matte, glm::normalize(glm::vec3(0, 0, 1))));
//    scene.shapes.push_back(new Plane(glm::vec3(0.0f, 0.0f, 100.0f), glm::vec3(1, 1, 1), matte, glm::normalize(glm::vec3(0, 0, -1))));
//
//    // Spheres
//    // scene.shapes.push_back(new Sphere(glm::vec3(30.0f, 0.0f, -70.0f), glm::vec3(1, 0, 0), phong, 12.0f));
//    // scene.shapes.push_back(new Sphere(glm::vec3(-40.0f, 0.0f, -70.0f), glm::vec3(0, 1, 0), phong, 18.0f));
//    // scene.shapes.push_back(new Sphere(glm::vec3(-25.0f, 15.0f, -40.0f), glm::vec3(0, 0, 1), phong, 4.0f));
//    // scene.shapes.push_back(new Sphere(glm::vec3(0.0f, 0.0f, -70.0f), glm::vec3(1, 1, 1), mirror, 16.0f));
//    // scene.shapes.push_back(new Sphere(glm::vec3(20.0f, 35.0f, -90.0f), glm::vec3(1, 1, 0), phong, 10.0f));
//    // scene.shapes.push_back(new Sphere(glm::vec3(15.0f, 10.0f, -40.0f), glm::vec3(1, 1, 1), refract, 10.0f));
//
//    // Meshes
//    // scene.shapes.push_back(new Mesh(glm::vec3(0.0f, 0.0f, -80.0f), glm::vec3(1, 1, 0), phong, "../res/sphere.obj", 15.0f));
//    scene.shapes.push_back(new Mesh(glm::vec3(0.0f, -20.0f, -80.0f), glm::vec3(1, 1, 0), mirror, "../res/teapot.obj", 5.0f));
//    // scene.shapes.push_back(new Mesh(glm::vec3(0.0f, -50.0f, -80.0f), glm::vec3(1, 1, 0), phong, "../res/bunny.obj", 5.0f));
//
//    // Lights
//    scene.lights.push_back(new PointLight(glm::vec3(50.0f, 50.0f, 30.0f), 50000, glm::vec3(1, 1, 0.25f)));
//    scene.lights.push_back(new PointLight(glm::vec3(-50.0f, 50.0f, 30.0f), 50000, glm::vec3(0.25f, 1, 1)));
//    scene.lights.push_back(new PointLight(glm::vec3(0.0f, 30.0f, -140.0f), 50000, glm::vec3(1, 1, 1)));
//
//    return scene;
//}
