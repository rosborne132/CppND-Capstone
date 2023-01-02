#include <iostream>

int main() {
    const int imageWidth = 256;
    const int imageHeight = 256;
    const double rgbValue = 255.999;

    std::cout << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";

    for (int j = imageHeight - 1; j >= 0; --j) {
        for (int i = 0; i < imageWidth; ++i) {
            auto r = double(i) / (imageWidth - 1);
            auto g = double(j) / (imageHeight - 1);
            auto b = 0.25;

            int ir = static_cast<int>(rgbValue * r);
            int ig = static_cast<int>(rgbValue * g);
            int ib = static_cast<int>(rgbValue * b);

            std::cout << ir << " " << ig << " " << ib << "\n";
        }
    }
}