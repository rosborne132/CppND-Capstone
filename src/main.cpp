#include <iostream>

#include "color.h"
#include "ray.h"
#include "vec3.h"

double hitSphere(const Point3& center, double radius, const Ray& r) {
    Vec3 oc = r.origin() - center;
    auto a = dot(r.direction(), r.direction());
    auto b = 2.0 * dot(oc, r.direction());
    auto c = dot(oc, oc) - radius * radius;
    auto discriminant = b * b - 4 * a * c;

    return (discriminant < 0)
        ? -1.0
        : (-b - sqrt(discriminant)) / (2.0 * a);
}

Color rayColor(const Ray& r) {
    auto t = hitSphere(Point3(0, 0, -1), 0.5, r);

    if (t > 0.0) {
        Vec3 N = unitVector(r.at(t) - Vec3(0, 0, -1));
        return 0.5 * Color(N.x() + 1, N.y() + 1, N.z() +1);
    }

    Vec3 unitDirection = unitVector(r.direction());
    t = 0.5 * (unitDirection.y() + 1.0);
    return (1.0 - t) * Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);
}

int main() {
    // Image
    const auto aspectRatio = 16.0 / 9.0;
    const int imageWidth = 400;
    const int imageHeight = static_cast<int>(imageWidth / aspectRatio);

    // Camera
    auto viewportHeight = 2.0;
    auto viewportWidth = aspectRatio * viewportHeight;
    auto focalLength = 1.0;

    auto origin = Point3(0, 0, 0);
    auto horizontal = Vec3(viewportWidth, 0, 0);
    auto vertical = Vec3(0, viewportHeight, 0);
    auto lowerLeftCorner = origin - horizontal / 2 - vertical / 2 - Vec3(0, 0, focalLength);

    // Render
    std::cout << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";

    for (int j = imageHeight - 1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << " " << std::flush;
        for (int i = 0; i < imageWidth; ++i) {
            auto u = static_cast<double>(i) / (imageWidth - 1);
            auto v = static_cast<double>(j) / (imageHeight - 1);
            Ray r{origin, lowerLeftCorner + u * horizontal + v * vertical - origin};
            Color pixelColor = rayColor(r);
            writeColor(std::cout, pixelColor);
        }
    }

    std::cerr << "\nDone. \n";
}
