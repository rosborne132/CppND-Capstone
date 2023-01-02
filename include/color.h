#ifndef COLOR_H
#define COLOR_H

#include <iostream>

#include "vec3.h"

#define RGB_VALUE 255.999

void writeColor(std::ostream &out, color pixelColor) {
    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(RGB_VALUE * pixelColor.x()) << " "
        << static_cast<int>(RGB_VALUE * pixelColor.y()) << " "
        << static_cast<int>(RGB_VALUE * pixelColor.z()) << "\n";
}

#endif