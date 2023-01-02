#include <iostream>

#include "color.h"
#include "vec3.h"

int main() {
    const int imageWidth = 256;
    const int imageHeight = 256;

    std::cout << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";

    for (int j = imageHeight - 1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << " " << std::flush;
        for (int i = 0; i < imageWidth; ++i) {
            color pixelColor(
                i / static_cast<double>(imageWidth - 1),
                j / static_cast<double>(imageHeight - 1),
                0.25
            );
            writeColor(std::cout, pixelColor);
        }
    }

    std::cerr << "\nDone. \n";
}
