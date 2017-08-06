//
// Created by Jack Purvis
//

#include <iostream>
#include <random>
#include <core/renderer.hpp>
#include <shapes/plane.hpp>
#include <shapes/sphere.hpp>
#include <materials/matte.hpp>
#include <materials/phong.hpp>

glm::vec3** Renderer::render(const int width, const int height) {
    glm::vec3** image = new glm::vec3*[width];
    for (int i = 0; i < width; i++) {
        image[i] = new glm::vec3[height];
    }

    // Setup scene
    Scene scene = makeScene();

    // Setup screen settings
    float aspectRatio = (float) width / (float) height;

    // Begin progress bar
    int totalSamples = width * height;
    int steps = 20;
    float percentSpace = 1.0f / (float) steps;
    float lastPercent = 0.0f;

    // Process every pixel
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            glm::vec2 pixelCamera;
            pixelCamera.x = (2 * ((x + 0.5f) / (float) width) - 1)  * tanf(fov / 2.0f * (float) M_PI / 180.0f) * aspectRatio;
            pixelCamera.y = (1 - 2 * ((y + 0.5f) / (float) height)) * tanf(fov / 2.0f * (float) M_PI / 180.0f);

            glm::vec3 colour;

            if (distributed) {
                for (int i = 0; i < 5; i++) {
                    Ray ray = Ray(0);
                    ray.origin = scene.cameraPosition;
                    float xOffset = ((rand() % 200) - 100) / 100.0f;
                    float yOffset = ((rand() % 200) - 100) / 100.0f;
                    ray.direction = glm::normalize(glm::vec3(pixelCamera.x + xOffset / 1000.0f, pixelCamera.y - yOffset / 1000.0f, -1.0f) - scene.cameraPosition);
                    colour += glm::clamp(castRay(ray, scene), 0.0f, 1.0f) * 0.2f;
                }
            } else {
                Ray ray = Ray(0);
                ray.origin = scene.cameraPosition;
                ray.direction = glm::normalize(glm::vec3(pixelCamera.x, pixelCamera.y, -1.0f) - scene.cameraPosition);
                colour = glm::clamp(castRay(ray, scene), 0.0f, 1.0f);
            }

            image[x][y] = colour;

            float percent = ((x * y) + y) / (float) totalSamples;
            if (percent > lastPercent + percentSpace) {
                lastPercent = percent;
                std::cout << "[" << ((int) (percent * 100)) << "%" << "]" << "\r" << std::flush;
            }
        }
    }

    // End progress bar
    std::cout << "[" << 100 << "%" << "]" << std::endl;

    return image;
}

glm::vec3 Renderer::castRay(Ray ray, Scene scene) {
    glm::vec3 radiance;

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

Scene Renderer::makeScene() {
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

    scene.lights.push_back(new PointLight(glm::vec3(50.0f, 50.0f, 10.0f), 25000.0f, glm::vec3(0.15f, 1, 1)));
    scene.lights.push_back(new PointLight(glm::vec3(-30.0f, 40.0f, -10.0f), 20000.0f, glm::vec3(1, 1, 0.15f)));

    return scene;
}
