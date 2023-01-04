#include <iostream>

#include "camera.h"
#include "constants.h"
#include "color.h"
#include "hittableList.h"
#include "material.h"
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

Color rayColor(const Ray& r, const Hittable& world, int depth) {
    HitRecord rec;

    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0) return Color(0,0,0);

    if (world.hit(r, 0.001, infinity, rec)) {
        Ray scattered;
        Color attenuation;
        if (rec.matPtr->scatter(r, rec, attenuation, scattered))
            return attenuation * rayColor(scattered, world, depth-1);
        return Color(0,0,0);
    }

    Vec3 unitDirection = unitVector(r.direction());
    auto t = 0.5 * (unitDirection.y() + 1.0);

    return (1.0 - t) * Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);
}

int main() {
    // Image
    const auto aspectRatio = 16.0 / 9.0;
    const int imageWidth = 400;
    const int imageHeight = static_cast<int>(imageWidth / aspectRatio);
    const int samplesPerPixel = 100;
    const int maxDepth = 50;

    // World
    HittableList world;
    auto materialGround = std::make_shared<Lambertian>(Color(0.8, 0.8, 0.0));
    auto materialCenter = std::make_shared<Lambertian>(Color(0.7, 0.3, 0.3));
    auto materialLeft   = std::make_shared<Metal>(Color(0.8, 0.8, 0.8), 0.3);
    auto materialRight  = std::make_shared<Metal>(Color(0.8, 0.6, 0.2), 1.0);

    world.add(std::make_shared<Sphere>(Point3(0.0, -100.5, -1.0), 100.0, materialGround));
    world.add(std::make_shared<Sphere>(Point3(0.0, 0.0, -1.0), 0.5, materialCenter));
    world.add(std::make_shared<Sphere>(Point3(-1.0, 0.0, -1.0), 0.5, materialLeft));
    world.add(std::make_shared<Sphere>(Point3(1.0, 0.0, -1.0), 0.5, materialRight));

    // Camera
    Camera cam;

    // Render
    std::cout << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";

    for (int j = imageHeight - 1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << " " << std::flush;
        for (int i = 0; i < imageWidth; ++i) {
            Color pixelColor(0, 0, 0);
            for (int s = 0; s < samplesPerPixel; ++s) {
                auto u = (i + randomDouble()) / (imageWidth-1);
                auto v = (j + randomDouble()) / (imageHeight-1);
                Ray r = cam.getRay(u, v);
                pixelColor += rayColor(r, world, maxDepth);
            }
            writeColor(std::cout, pixelColor, samplesPerPixel);
        }
    }

    std::cerr << "\nDone. \n";
}
