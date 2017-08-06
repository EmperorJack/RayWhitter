#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <jpge.h>
#include <scene.hpp>
#include <plane.hpp>
#include <sphere.hpp>
#include <matte.hpp>
#include <phong.hpp>

const int imageWidth = 1920;
const int imageHeight = 1080;
glm::vec3 image[imageWidth][imageHeight];
uint8_t data[imageHeight][imageWidth * 3];
const int maxBounces = 10;
glm::vec3 backgroundColour = glm::vec3(0.8f);
glm::vec3 shadowColour = glm::vec3(0.0f);

glm::vec3 castRay(Ray ray, Scene scene) {
    glm::vec3 radiance = glm::vec3(0, 0, 0);

    // Check for intersection
    Intersection intersect = scene.intersect(ray);

    if (intersect.hit) {

        glm::vec3 intersectPoint = ray.position(intersect.t);

        // Compute lighting contribution from each light
        for (PointLight *light : scene.lights) {

            glm::vec3 lightDirection, lightIntensity;
            float lightDistance;
            light->illuminate(intersectPoint, lightDirection, lightIntensity, lightDistance);

            float a = std::max(glm::dot(intersect.normal, -lightDirection), 0.0f);
            if (a <= 0.0f) continue;

            // Cast a shadow ray to the light
            Ray shadowRay = Ray(0);
            shadowRay.direction = -lightDirection;
            shadowRay.origin = intersectPoint + 1.0f * shadowRay.direction;
            Intersection shadowIntersect = scene.intersect(shadowRay);

            // Check if the light is shadowed
            bool visible = !(shadowIntersect.hit && shadowIntersect.t < lightDistance);

            radiance += intersect.shape->material->evaluate(ray, intersect, lightDirection, lightIntensity, a) * (visible ? glm::vec3(1) : shadowColour);
        }

        // Cast reflection ray if needed
        if (ray.depth < maxBounces && intersect.shape->material->kr > 0.0f) {
            glm::vec3 reflect = glm::reflect(ray.direction, intersect.normal);

            Ray reflectRay = Ray(ray.depth + 1);
            reflectRay.direction = reflect;
            reflectRay.origin = intersectPoint + 1.0f * reflectRay.direction;

            radiance += intersect.shape->material->kr * castRay(reflectRay, scene);
        }
    } else {
        radiance = backgroundColour;
    }

    return radiance;
}

Scene makeScene() {
    glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 0.0f);

    Scene scene = Scene(cameraPosition);

    Matte* matte = new Matte(0.0f, 1.0f);
    Phong* phong = new Phong(0.0f, 0.8f, 0.2f, 12);
    Phong* mirror = new Phong(0.6f, 0.0f, 0.4f, 8);
    Matte* mirrorGround = new Matte(0.3f, 0.7f);

    scene.shapes.push_back(new Plane(glm::vec3(0.0f, -25.0f, 0.0f), glm::vec3(1, 1, 1), mirrorGround, glm::normalize(glm::vec3(0, 1, 0))));

    scene.shapes.push_back(new Sphere(glm::vec3(30.0f, 0.0f, -60.0f), glm::vec3(1, 0, 0), phong, 12.0f));
    scene.shapes.push_back(new Sphere(glm::vec3(-40.0f, 0.0f, -70.0f), glm::vec3(0, 1, 0), phong, 20.0f));
    scene.shapes.push_back(new Sphere(glm::vec3(-25.0f, 15.0f, -40.0f), glm::vec3(0, 0, 1), phong, 4.0f));
    scene.shapes.push_back(new Sphere(glm::vec3(0.0f, 10.0f, -70.0f), glm::vec3(1, 1, 1), mirror, 14.0f));
    scene.shapes.push_back(new Sphere(glm::vec3(20.0f, 30.0f, -70.0f), glm::vec3(1, 1, 0), matte, 8.0f));
    scene.shapes.push_back(new Sphere(glm::vec3(0.0f, 10.0f, 70.0f), glm::vec3(1, 0, 1), phong, 16.0f));

    scene.lights.push_back(new PointLight(glm::vec3(0.0f, 50.0f, 10.0f), 80000.0f, glm::vec3(1, 1, 1)));

    return scene;
}

int main() {
    std::cout << "Whitted Ray Tracer" << std::endl;

    // Setup scene
    Scene scene = makeScene();

    // Setup screen settings
    float aspectRatio = (float) imageWidth / (float) imageHeight;
    float fov = 90.0f;

    // Begin progress bar
    int totalSamples = imageWidth * imageHeight;
    int steps = 20;
    float percentSpace = 1.0f / (float) steps;
    float lastPercent = 0.0f;

    // Process every pixel
    for (int x = 0; x < imageWidth; x++) {
        for (int y = 0; y < imageHeight; y++) {
            glm::vec2 pixelCamera;
            pixelCamera.x = (2 * ((x + 0.5f) / (float) imageWidth) - 1)  * tanf(fov / 2.0f * (float) M_PI / 180.0f) * aspectRatio;
            pixelCamera.y = (1 - 2 * ((y + 0.5f) / (float) imageHeight)) * tanf(fov / 2.0f * (float) M_PI / 180.0f);

            Ray ray = Ray(0);
            ray.origin = scene.cameraPosition;
            ray.direction = glm::normalize(glm::vec3(pixelCamera.x, pixelCamera.y, -1.0f) - scene.cameraPosition);

            image[x][y] = glm::clamp(castRay(ray, scene), 0.0f, 1.0f);

            float percent = ((x * y) + y) / (float) totalSamples;
            if (percent > lastPercent + percentSpace) {
                lastPercent = percent;
                std::cout << "[" << ((int) (percent * 100)) << "%" << "]" << "\r" << std::flush;
            }
        }
    }

    // End progress bar
    std::cout << "[" << 100 << "%" << "]" << std::endl;

    std::cout << "Writing image file...";

    // Output image file
    for (int y = 0; y < imageHeight; y++) {
        for (int x = 0; x < imageWidth; x++) {
            data[y][x * 3] = (uint8_t) (image[x][y].r * 255.0f);
            data[y][x * 3 + 1] = (uint8_t) (image[x][y].g * 255.0f);
            data[y][x * 3 + 2] = (uint8_t) (image[x][y].b * 255.0f);
        }
    }

    // Writes JPEG image to a file.
    // num_channels must be 1 (Y), 3 (RGB), or 4 (RGBA), image pitch must be width*num_channels.
    // bool compress_image_to_jpeg_file(const char *pFilename, int width, int height, int num_channels, const uint8 *pImage_data, const params &comp_params = params());
    jpge::compress_image_to_jpeg_file("test.jpg", imageWidth, imageHeight, 3, *data);

    std::cout << " Finished" << std::endl;

    return 0;
}
