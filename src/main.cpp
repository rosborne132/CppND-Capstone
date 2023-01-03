#include <iostream>

#include "constants.h"
#include "color.h"
#include "hittableList.h"
#include "sphere.h"

double hitSphere(const Point3& center, double radius, const Ray& r) {
    Vec3 oc = r.origin() - center;
    auto a = r.direction().lengthSquared();
    auto halfB = dot(oc, r.direction());
    auto c = oc.lengthSquared() - radius * radius;
    auto discriminant = halfB * halfB - a * c;

    return (discriminant < 0)
        ? -1.0
        : (-halfB - sqrt(discriminant)) / a;
}

Color rayColor(const Ray& r, const Hittable& world) {
    HitRecord rec;
    if (world.hit(r, 0, infinity, rec)) return 0.5 * (rec.normal + Color(1, 1, 1));

    Vec3 unitDirection = unitVector(r.direction());
    auto t = 0.5 * (unitDirection.y() + 1.0);
    return (1.0 - t) * Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);
}

int main() {
    // Image
    const auto aspectRatio = 16.0 / 9.0;
    const int imageWidth = 400;
    const int imageHeight = static_cast<int>(imageWidth / aspectRatio);

    // World
    HittableList world;
    world.add(make_shared<Sphere>(Point3(0, 0, -1), 0.5));
    world.add(make_shared<Sphere>(Point3(0, -100.5, -1), 100));

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
            Color pixelColor = rayColor(r, world);
            writeColor(std::cout, pixelColor);
        }
    }

    std::cerr << "\nDone. \n";
}
