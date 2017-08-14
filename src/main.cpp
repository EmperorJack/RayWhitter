//
// Created by Jack Purvis
//

#include <iostream>
#include <glm/glm.hpp>
#include <jpge.h>
#include <core/renderer.hpp>

void parseSceneFile(char* sceneFilename, Renderer &renderer, Scene &scene, int &imageWidth, int &imageHeight);

int main(int argc, char *argv[]) {
    fprintf(stdout, "RayWhitter - A Whitted Ray Tracer\n");

    if (argc != 3) {
        fprintf(stderr, "A scene filename and image output name must be provided!\n");
        fprintf(stderr, "e.g: ./RayWhitter \"../res/teapot.txt\" \"test.jpg\"\n");
        return -1;
    }

    char* sceneFilename = argv[1];
    char* outFilename = argv[2];

    int imageWidth, imageHeight;
    Renderer renderer;
    Scene scene;

    // Parse the scene file resulting in a renderer object, scene object and image dimensions
    parseSceneFile(sceneFilename, renderer, scene, imageWidth, imageHeight);

    glm::vec3** image = renderer.render(imageWidth, imageHeight, scene);

    fprintf(stdout, "Writing image file...\n");

    uint8_t* data = new uint8_t[imageHeight * imageWidth * 3];

    // Convert the colour vectors into the required data format
    for (int y = 0; y < imageHeight; y++) {
        for (int x = 0; x < imageWidth; x++) {

            // Compute 1d indexes for each colour channel of this pixel
            int r = imageWidth * (y * 3) + (x * 3);
            int g = imageWidth * (y * 3) + (x * 3 + 1);
            int b = imageWidth * (y * 3) + (x * 3 + 2);

            data[r] = (uint8_t) (image[x][y].r * 255.0f);
            data[g] = (uint8_t) (image[x][y].g * 255.0f);
            data[b] = (uint8_t) (image[x][y].b * 255.0f);
        }
    }

    // Write the image data to a JPEG file
    jpge::params params;
    params.m_quality = 100;
    jpge::compress_image_to_jpeg_file(outFilename, imageWidth, imageHeight, 3, data, params);

    delete[] image;
    delete[] data;

    fprintf(stdout, "Finished...\n");

    return 0;
}
