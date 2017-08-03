#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <jpge.h>
#include <scene.hpp>

const int imageWidth = 512 * 0.5f;
const int imageHeight = 512 * 0.5f;
glm::vec3 image[imageWidth][imageHeight];

glm::vec3 getLighting(Ray ray, Scene scene) {
    glm::vec3 radiance = glm::vec3(0, 0, 0);

    // Check for intersection
    Intersection intersect = scene.intersect(ray);

    if (intersect.hit) {

        glm::vec3 intersectPoint = ray.origin + ray.direction * intersect.t;

        // Get intersection normal
        glm::vec3 normal = intersect.shape->getNormal(intersectPoint);

        // Compute radiance
        for (PointLight *light : scene.lights) {

            glm::vec3 lightDirection, lightIntensity;
            light->illuminate(intersectPoint, lightDirection, lightIntensity, intersect.t);

            // Cast a ray to the light
            Ray shadowRay;
            shadowRay.origin = intersectPoint + normal;
            shadowRay.direction = -lightDirection;
            Intersection shadowIntersect = scene.intersect(shadowRay);

//            std::cout << lightIntensity.x << std::endl;
            if (shadowIntersect.hit) {
//                std::cout << intersect.shape->id << " -> " << shadowIntersect.shape->id << std::endl;
//                radiance.r = 1.0f;
//                radiance.g = 1.0f;
                continue;
            }

            float a = std::max(glm::dot(normal, -lightDirection), 0.0f);
            glm::vec3 li = lightIntensity * intersect.shape->getColour() * a;
//            std::cout << a << std::endl;
            if (a == 0.0f) radiance.g = 1.0f;
            radiance += li;
        }
    }

    return radiance;
}

Scene makeScene() {
    glm::vec3 eyePosition = glm::vec3(0.0f, 100.0f, -250.0f);

    Scene scene = Scene(eyePosition);

    // Scene A
    scene.shapes.push_back(new Sphere(glm::vec3(-120.0f, -50.0f, 0.0f), 40.0f, 0));
    scene.shapes.push_back(new Sphere(glm::vec3(120.0f, 0.0f, 0.0f), 50.0f, 1));
    scene.shapes.push_back(new Sphere(glm::vec3(0.0f, -80.0f, -35.0f), 25.0f, 2));

    scene.shapes.push_back(new Plane(glm::vec3(0.0f, 0.0f, 0.0f), glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)), 3));

    scene.lights.push_back(new PointLight(glm::vec3(-15.0f, -100.0f, -50.0f), 1.0f, glm::vec3(1.0f, 1.0f, 1.0f)));
//    scene.shapes.push_back(new Sphere(glm::vec3(-20.0f, -100.0f, -50.0f), 5.0f, 4));

    // Scene B
//    scene.shapes.push_back(new Sphere(glm::vec3(0.0f, 0.0f, 0.0f), 25.0f, 2));
//    scene.lights.push_back(new PointLight(glm::vec3(0.0f, -100.0f, 0.0f), 1.0f, glm::vec3(1.0f, 1.0f, 1.0f)));

    // Scene C
//    scene.lights.push_back(new PointLight(glm::vec3(-50.0f, 0.0f, 0.0f), 1.0f, glm::vec3(1.0f, 1.0f, 1.0f)));
//    scene.shapes.push_back(new Sphere(glm::vec3(-50.0f, 0.0f, -100.0f), 5.0f, 4));
//    scene.shapes.push_back(new Sphere(glm::vec3(0.0f, 0.0f, 0.0f), 20.0f, 0));
//    scene.shapes.push_back(new Sphere(glm::vec3(50.0f, 0.0f, 0.0f), 50.0f, 1));

    return scene;
}

int main() {
    std::cout << "Whitted Ray Tracer" << std::endl;

    // Setup scene
    Scene scene = makeScene();

    // Setup image buffer
    glm::vec2 pixelScale = glm::vec2(512.0f / (float) imageWidth, 512.0f / (float) imageHeight);

    // Begin progress bar
    int totalSamples = imageWidth * imageHeight;
    int steps = 20;
    float percentSpace = 1.0f / (float) steps;
    float lastPercent = 0.0f;

    // Process every pixel
    for (int x = 0; x < imageWidth; x++) {
        for (int y = 0; y < imageHeight; y++) {
            glm::vec2 pixelPosition = glm::vec2(x - imageWidth / 2, y - imageWidth / 2) * pixelScale;
            pixelPosition += glm::vec2(scene.cameraPosition.x, scene.cameraPosition.y);

            Ray ray;
            ray.origin = scene.cameraPosition;
            ray.direction = glm::normalize(scene.cameraPosition - glm::vec3(pixelPosition, 0.0f));

            image[x][y] = getLighting(ray, scene);

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
            data[y][x * 3] = (uint8_t) (image[x][y].x * 255.0f);
            data[y][x * 3 + 1] = (uint8_t) (image[x][y].y * 255.0f);
            data[y][x * 3 + 2] = (uint8_t) (image[x][y].z * 255.0f);
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