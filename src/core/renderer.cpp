//
// Created by Jack Purvis
//

#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>
#include <iostream>
#include <core/renderer.hpp>
#include <core/constants.hpp>
#include <core/material.hpp>

double clamp(double upper, double lower, double x) {
    return std::min(upper, std::max(x, lower));
}

glm::vec3** Renderer::render(int width, int height, Scene scene) {
    bool antiAliasing = antiAliasingAmount > 1;
    if (antiAliasing) {
        width *= antiAliasingAmount;
        height *= antiAliasingAmount;
    }

    glm::vec3** image = new glm::vec3*[width];
    for (int i = 0; i < width; i++) {
        image[i] = new glm::vec3[height];
    }

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

            Ray ray = Ray(0);
            ray.origin = scene.cameraPosition;
            ray.direction = glm::normalize(glm::vec3(pixelCamera.x, pixelCamera.y, -1.0f) - scene.cameraPosition);
            colour = glm::clamp(castRay(scene, ray), 0.0f, 1.0f);

            image[x][y] = colour;

            float percent = ((x * y) + y) / (float) totalSamples;
            if (percent > lastPercent + percentSpace) {
                lastPercent = percent;
                std::cout << "[" << ((int) (percent * 100)) << "%" << "]" << "\r" << std::flush;
            }
        }
    }

    // Reduce larger image to original size if anti-aliasing was enabled
    if (antiAliasing) {
        int actualWidth = width / antiAliasingAmount;
        int actualHeight = height / antiAliasingAmount;

        glm::vec3** reducedImage = new glm::vec3*[actualWidth];
        for (int i = 0; i < actualWidth; i++) {
            reducedImage[i] = new glm::vec3[actualHeight];
        }

        // Sum the samples for each pixel
        for (int x = 0; x < width; x++) {
            for (int y = 0; y < height; y++) {
                int i = (int) (x / (float) antiAliasingAmount);
                int j = (int) (y / (float) antiAliasingAmount);
                reducedImage[i][j] += image[x][y];
            }
        }

        // Average the samples for each pixel
        for (int i = 0; i < actualWidth; i++) {
            for (int j = 0; j < actualHeight; j++) {
                reducedImage[i][j] /= (float) (antiAliasingAmount * antiAliasingAmount);
            }
        }

        image = reducedImage;
    }

    // End progress bar
    std::cout << "[" << 100 << "%" << "]" << std::endl;

    return image;
}

glm::vec3 Renderer::castRay(Scene scene, Ray ray) {
    glm::vec3 radiance;

    // Check for intersection
    Intersection intersect = scene.intersect(ray, useBoundingBox);

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
            Intersection shadowIntersect = scene.intersect(shadowRay, useBoundingBox);

            // Check if the light is shadowed
            bool visible = !(shadowIntersect.hit && shadowIntersect.t < lightDistance);

            radiance += evaluatePhong(ray, intersect, lightDirection, lightIntensity, cosLightDirection) * (visible ? glm::vec3(1) : shadowColour);
        }

        // Cast reflection and refraction rays
        if (ray.depth < maxBounces) {
            Material* material = intersect.shape->material;

            float kr, kt;
            kr = material->kr;
            kt = material->kt;
            if (kt > 0.0f) {
                fresnel(ray, intersect, material->ior, kr);

                // If not total internal reflection
                if (kr < 1.0f) {
                    kt = 1.0f - kr;
                } else {
                    kr = 1.0f;
                    kt = 0.0f;
                }
            }

            if (kr > 0.0f) radiance += kr * evaluateReflection(scene, ray, intersect);
            if (kt > 0.0f) radiance += kt * evaluateRefraction(scene, ray, intersect);
        }
    } else {
        radiance = backgroundColour;
    }

    return radiance;
}

glm::vec3 Renderer::evaluatePhong(Ray ray, Intersection intersect, glm::vec3 lightDirection, glm::vec3 lightIntensity, float a) {
    Material* material = intersect.shape->material;

    glm::vec3 diffuse = lightIntensity * intersect.shape->albedo * a;

    glm::vec3 specular;
    if (material->ks > 0.0f) {
        glm::vec3 reflect = glm::reflect(lightDirection, intersect.normal);
        specular = lightIntensity * std::pow(std::max(0.0f, glm::dot(reflect, -ray.direction)), material->n);
    }

    return diffuse * material->kd + specular * material->ks;
}

glm::vec3 Renderer::evaluateReflection(Scene scene, Ray ray, Intersection intersect) {
    glm::vec3 reflect = glm::reflect(ray.direction, intersect.normal);

    Ray reflectRay = Ray(ray.depth + 1);
    reflectRay.direction = reflect;
    reflectRay.origin = intersect.point + 1.0f * reflectRay.direction;

    return castRay(scene, reflectRay);
}

glm::vec3 Renderer::evaluateRefraction(Scene scene, Ray ray, Intersection intersect) {
    float cosRayDirection = clamp(-1, 1, glm::dot(ray.direction, intersect.normal));

    float iorLeaving = 1;
    float iorEntering = intersect.shape->material->ior;

    glm::vec3 normal = glm::vec3(intersect.normal);

    if (cosRayDirection < 0) {
        // Entering the medium
        cosRayDirection = -cosRayDirection;
    } else {
        // Leaving the medium
        std::swap(iorLeaving, iorEntering);
        normal = -normal;
    }

    float iorRatio = iorLeaving / iorEntering;

    float k = 1 - iorRatio * iorRatio * (1 - cosRayDirection * cosRayDirection);

    glm::vec3 refractDirection = k < 0 ? glm::vec3() :
                                 iorRatio * ray.direction + (iorRatio * cosRayDirection - sqrtf(k)) * normal;

    Ray refractRay = Ray(ray.depth + 1);
    refractRay.direction = glm::normalize(refractDirection);
    refractRay.origin = intersect.point + 1.0f * refractRay.direction;

    return castRay(scene, refractRay);
}

void Renderer::fresnel(Ray ray, Intersection intersect, float ior, float &kr) {
    float cosRayDirection = clamp(-1, 1, glm::dot(ray.direction, intersect.normal));

    float iorLeaving = 1;
    float iorEntering = intersect.shape->material->ior;

    if (cosRayDirection > 0) {
        // Leaving the medium
        std::swap(iorLeaving, iorEntering);
    }

    // Get sin of ray angle using Snell's law
    float sinRayDirection = iorLeaving / iorEntering * sqrtf(std::max(0.0f, 1 - cosRayDirection * cosRayDirection));

    if (sinRayDirection >= 1) {
        // Total internal reflection
        kr = 1;
    } else {
        float cost = sqrtf(std::max(0.0f, 1 - sinRayDirection * sinRayDirection));
        cosRayDirection = abs(cosRayDirection);

        float rs = ((iorLeaving * cosRayDirection) - (iorEntering * cost)) / ((iorLeaving * cosRayDirection) + (iorEntering * cost));
        float rp = ((iorEntering * cosRayDirection) - (iorLeaving * cost)) / ((iorEntering * cosRayDirection) + (iorLeaving * cost));
        kr = (rs * rs + rp * rp) / 2;
    }
}
