#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <scene.hpp>

int imageWidth = 64;//640;
int imageHeight = 48;//480;

glm::vec3 getLighting(Ray ray, Scene scene, float maxDepth) {
    glm::vec3 contribution = glm::vec3(0, 0, 0);

    // Until the ray hits something or exceeds the maximum depth
    while(ray.time < maxDepth) {

        // Check for intersection
        if (scene.intersect(ray)) {

            // Handle intersection
            contribution.x = 255.0f;

            break;
        } else {

            // Advance ray
            ray.time += 0.1f;
        }
    }

    return contribution;
}

Scene makeScene() {
    std::vector<Sphere> spheres;
    spheres.push_back(Sphere(glm::vec3(0.0f, 0.0f, 30.0f), 15.0f));

    std::vector<Plane> planes;

    std::vector<PointLight> lights;

    glm::vec3 eyePosition = glm::vec3(0.0f, 0.0f, -25.0f);

    Scene scene = Scene(eyePosition, planes, spheres, lights);

    return scene;
}

int main() {
    std::cout << "Whitted Ray Tracer" << std::endl;

    // Setup scene
    Scene scene = makeScene();

    // Setup image buffer
    glm::vec3 image[imageWidth][imageHeight];
    float maxDepth = 100.0f;

    // Process every pixel
    for (int x = 0; x < imageWidth; x++) {
        for (int y = 0; y < imageHeight; y++) {
            glm::vec2 pixelPosition = glm::vec2(x - imageWidth / 2, y - imageWidth / 2);

            Ray ray;
            ray.origin = glm::vec3(scene.cameraPosition);
            ray.direction = glm::vec3(pixelPosition, 0.0f) - scene.cameraPosition;
            ray.time = 0.0f;

            image[x][y] = getLighting(ray, scene, maxDepth);
        }
    }

    // Output image file
    for (int y = 0; y < imageHeight; y++) {
        for (int x = 0; x < imageWidth; x++) {
            //std::cout << "[" << image[x][y].x << "," << image[x][y].y << "," << image[x][y].z << "] ";
            std::cout << (image[x][y].x > 0.0f ? 1 : 0);
        }
        std::cout << std::endl;
    }

    return 0;
}