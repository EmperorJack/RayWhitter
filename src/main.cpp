#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <jpge.h>
#include <scene.hpp>

const int imageWidth = 512;
const int imageHeight = 512;
glm::vec3 image[imageWidth][imageHeight];

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
    glm::vec3 eyePosition = glm::vec3(0.0f, 0.0f, -25.0f);

    std::vector<Sphere> spheres;
    spheres.push_back(Sphere(glm::vec3(0.0f, 0.0f, 30.0f), 20.0f));
    spheres.push_back(Sphere(glm::vec3(-60.0f, 0.0f, 70.0f), 10.0f));

    std::vector<Plane> planes;
    planes.push_back(Plane(glm::vec3(60.0f, 0.0f, 50.0f), 10.0f, 30.0f));

    std::vector<PointLight> lights;

    Scene scene = Scene(eyePosition, planes, spheres, lights);

    return scene;
}

int main() {
    std::cout << "Whitted Ray Tracer" << std::endl;

    // Setup scene
    Scene scene = makeScene();

    // Setup image buffer
    glm::vec2 pixelScale = glm::vec2(50.0f / (float) imageWidth, 50.0f / (float) imageHeight);
    float maxDepth = 100.0f;

    // Begin progress bar
    int totalSamples = imageWidth * imageHeight;
    int steps = 20;
    float percentSpace = 1.0f / (float) steps;
    float lastPercent = 0.0f;

    // Process every pixel
    for (int x = 0; x < imageWidth; x++) {
        for (int y = 0; y < imageHeight; y++) {
            glm::vec2 pixelPosition = glm::vec2(x - imageWidth / 2, y - imageWidth / 2) * pixelScale;

            Ray ray;
            ray.origin = glm::vec3(scene.cameraPosition);
            ray.direction = glm::vec3(pixelPosition, 0.0f) - scene.cameraPosition;
            ray.time = 0.0f;

            image[x][y] = getLighting(ray, scene, maxDepth);

            float percent = ((x * y) + y) / (float) totalSamples;
            if (percent > lastPercent + percentSpace) {
                lastPercent = percent;
                std::cout << "[" << ((int) (percent * 100)) << "%" << "]" << "\r" << std::flush;
            }
        }
    }

    // End progress bar
    std::cout << "[" << 100 << "%" << "]" << std::endl;

    uint8_t data[imageHeight][imageWidth * 3];

    // Output image file
    for (int y = 0; y < imageHeight; y++) {
        for (int x = 0; x < imageWidth; x++) {
            //float val = image[x][y].x;
            //std::cout << (val > 0.0f ? "11" : "00");
            data[y][x * 3] = (uint8_t) (image[x][y].x);
            data[y][x * 3 + 1] = (uint8_t) (image[x][y].y);
            data[y][x * 3 + 2] = (uint8_t) (image[x][y].z);
        }
        //std::cout << std::endl;
    }

    std::cout << "Writing image file...";

    // Writes JPEG image to a file.
    // num_channels must be 1 (Y), 3 (RGB), or 4 (RGBA), image pitch must be width*num_channels.
    // bool compress_image_to_jpeg_file(const char *pFilename, int width, int height, int num_channels, const uint8 *pImage_data, const params &comp_params = params());
    jpge::compress_image_to_jpeg_file("test.jpg", imageWidth, imageHeight, 3, *data);

    std::cout << " Finished" << std::endl;
    
    return 0;
}