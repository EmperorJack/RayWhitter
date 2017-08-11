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
#include <materials/reflective.hpp>
#include <materials/refractive.hpp>

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

        // Compute lighting contribution from each light
        for (PointLight *light : scene.lights) {

            glm::vec3 lightDirection, lightIntensity;
            float lightDistance;
            light->illuminate(intersect.point, lightDirection, lightIntensity, lightDistance);

            float cosLightDirection = std::max(glm::dot(intersect.normal, -lightDirection), 0.0f);
            if (cosLightDirection <= 0.0f) continue;

            // Cast a shadow ray to the light
            Ray shadowRay = Ray(0);
            shadowRay.direction = -lightDirection;
            shadowRay.origin = intersect.point + 1.0f * shadowRay.direction;
            Intersection shadowIntersect = scene.intersect(shadowRay);

            // Check if the light is shadowed
            bool visible = !(shadowIntersect.hit && shadowIntersect.t < lightDistance);

            radiance += intersect.shape->material->evaluate(ray, intersect, lightDirection, lightIntensity, cosLightDirection) * (visible ? glm::vec3(1) : shadowColour);
        }

        // Cast reflection and refraction rays
        if (ray.depth < maxBounces) {
            radiance += intersect.shape->material->evaluateReflection(this, scene, ray, intersect);
            radiance += intersect.shape->material->evaluateRefraction(this, scene, ray, intersect);
        }
    } else {
        radiance = backgroundColour;
    }

    return radiance;
}

Scene Renderer::makeScene() {
    glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 0.0f);

    Scene scene = Scene(cameraPosition);

    Matte* matte = new Matte(1);
    Phong* phong = new Phong(0.8f, 0.2f, 12);
    Reflective* mirror = new Reflective(0, 0.2f, 8, 0.8f);
    Reflective* mirrorGround = new Reflective(0.7f, 0, 0, 0.3f);
    Refractive* glass = new Refractive(0, 0, 8, 0, 1, 1.1f);
    Refractive* refract = new Refractive(0, 0, 0, 0.2f, 0.8f, 1.1f);

    scene.shapes.push_back(new Plane(glm::vec3(0.0f, -120.0f, 0.0f), glm::vec3(1, 1, 1), mirrorGround, glm::normalize(glm::vec3(0, 1, 0))));
    scene.shapes.push_back(new Plane(glm::vec3(0.0f, 120.0f, 0.0f), glm::vec3(1, 1, 1), mirrorGround, glm::normalize(glm::vec3(0, -1, 0))));
    scene.shapes.push_back(new Plane(glm::vec3(200.0f, 0.0f, 0.0f), glm::vec3(1, 0, 0), mirrorGround, glm::normalize(glm::vec3(-1, 0, 0))));
    scene.shapes.push_back(new Plane(glm::vec3(-200.0f, 0.0f, 0.0f), glm::vec3(0, 1, 0), mirrorGround, glm::normalize(glm::vec3(1, 0, 0))));
    scene.shapes.push_back(new Plane(glm::vec3(0.0f, 0.0f, -200.0f), glm::vec3(0, 0, 1), mirrorGround, glm::normalize(glm::vec3(0, 0, 1))));
    scene.shapes.push_back(new Plane(glm::vec3(0.0f, 0.0f, 100.0f), glm::vec3(1, 1, 1), mirrorGround, glm::normalize(glm::vec3(0, 0, -1))));

    scene.shapes.push_back(new Sphere(glm::vec3(30.0f, 0.0f, -70.0f), glm::vec3(1, 0, 0), phong, 12.0f));
    scene.shapes.push_back(new Sphere(glm::vec3(-40.0f, 0.0f, -70.0f), glm::vec3(0, 1, 0), phong, 18.0f));
    scene.shapes.push_back(new Sphere(glm::vec3(-25.0f, 15.0f, -40.0f), glm::vec3(0, 0, 1), phong, 4.0f));
    scene.shapes.push_back(new Sphere(glm::vec3(0.0f, 0.0f, -70.0f), glm::vec3(1, 1, 1), mirror, 16.0f));
    scene.shapes.push_back(new Sphere(glm::vec3(20.0f, 35.0f, -90.0f), glm::vec3(1, 1, 0), phong, 10.0f));
    scene.shapes.push_back(new Sphere(glm::vec3(15.0f, 10.0f, -40.0f), glm::vec3(1, 1, 1), refract, 10.0f));

    scene.lights.push_back(new PointLight(glm::vec3(50.0f, 50.0f, 10.0f), 50000, glm::vec3(1, 1, 0.25f)));
    scene.lights.push_back(new PointLight(glm::vec3(-30.0f, 40.0f, 10.0f), 50000, glm::vec3(0.25f, 1, 1)));
    scene.lights.push_back(new PointLight(glm::vec3(0.0f, 30.0f, -120.0f), 50000, glm::vec3(1, 1, 1)));

    return scene;
}
