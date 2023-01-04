#ifndef COLOR_H
#define COLOR_H

#include <iostream>

#include "vec3.h"

#define RGB_VALUE 256

void writeColor(std::ostream &out, Color pixelColor, int samplesPerPixel) {
    auto r = pixelColor.x();
    auto g = pixelColor.y();
    auto b = pixelColor.z();

    // Divide the color by the number of samples and gamma-correct for gamma=2.0.
    auto scale = 1.0 / samplesPerPixel;
    r = sqrt(scale * r);
    g = sqrt(scale * g);
    b = sqrt(scale * b);

    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(RGB_VALUE * clamp(r, 0.0, 0.999)) << ' '
        << static_cast<int>(RGB_VALUE * clamp(g, 0.0, 0.999)) << ' '
        << static_cast<int>(RGB_VALUE * clamp(b, 0.0, 0.999)) << '\n';
}

#endif