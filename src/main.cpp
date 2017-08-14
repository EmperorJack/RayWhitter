//
// Created by Jack Purvis
//

#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <jpge.h>
#include <core/constants.hpp>
#include <core/renderer.hpp>

int main() {
    std::cout << "Whitted Ray Tracer" << std::endl;

    const int imageWidth = 960;
    const int imageHeight = 540;

    Renderer renderer;
    glm::vec3** image = renderer.render(imageWidth, imageHeight);

    std::cout << "Writing image file...";

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
    jpge::compress_image_to_jpeg_file("test.jpg", imageWidth, imageHeight, 3, data, params);

    delete[] image;
    delete[] data;

    std::cout << " Finished" << std::endl;

    return 0;
}
