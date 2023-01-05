#include <iostream>
#include <chrono>
#include <memory>
#include <thread>
#include <vector>

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

HittableList random_scene() {
    HittableList world;

    auto groundMaterial = std::make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
    world.add(std::make_shared<Sphere>(Point3(0, -1000, 0), 1000, groundMaterial));

    // TODO: can optimize by using threads
    // TODO: update ot make the centers more random
    int numOfSphere = 2;
    for (int a = 0; a < numOfSphere + 1; a++) {
        auto chooseMat = randomDouble();
        Point3 center(a + 0.9 * randomDouble(), 0.2,  0.9 * randomDouble());
        double sphereSize = 0.2;

        if ((center - Point3(4, 0.2, 0)).length() > 0.9) {
            std::shared_ptr<Material> sphereMaterial;

            if (chooseMat < 0.8) {
                // diffuse
                auto albedo = Color::random() * Color::random();
                sphereMaterial = std::make_shared<Lambertian>(albedo);
                world.add(make_shared<Sphere>(center, sphereSize, sphereMaterial));
            } else if (chooseMat < 0.95) {
                // metal
                auto albedo = Color::random(0.5, 1);
                auto fuzz = randomDouble(0, 0.5);
                sphereMaterial = std::make_shared<Metal>(albedo, fuzz);
                world.add(std::make_shared<Sphere>(center, sphereSize, sphereMaterial));
            } else {
                // glass
                sphereMaterial = std::make_shared<Dielectric>(1.5);
                world.add(std::make_shared<Sphere>(center, sphereSize, sphereMaterial));
            }
        }
    }

    return world;
}

int main() {
    // Image
    const auto aspectRatio = 3.0 / 2.0;
    const int imageWidth = 1200;
    const int imageHeight = static_cast<int>(imageWidth / aspectRatio);
    // TODO: add loading counter
    const int samplesPerPixel = 50;
    const int maxDepth = 50;

    // World
    auto world = random_scene();

    // Camera
    Point3 lookfrom(13, 2, 3);
    Point3 lookat(0, 0, 0);
    Vec3 vup(0, 1, 0);
    auto distToFocus = 10.0;
    auto aperture = 0.1;

    Camera cam(lookfrom, lookat, vup, 20, aspectRatio, aperture, distToFocus);

    auto start = std::chrono::high_resolution_clock::now();

    // Render
    // TODO: Add more threads. 4 total.
    std::shared_ptr<std::vector<Color>> pointers[2] = {
        std::make_shared<std::vector<Color>>(),
        std::make_shared<std::vector<Color>>()
    };

    // create thread that loops through the first half of the picture
    // Go through the top half
    // TODO: refactor into lambda
    std::thread t1([cam, world, pointers, imageHeight, imageWidth]() mutable {
        for (int j = imageHeight - 1; j >= (imageHeight / 2); --j) {
            for (int i = 0; i < imageWidth; ++i) {
                Color pixelColor(0, 0, 0);
                for (int s = 0; s < samplesPerPixel; ++s) {
                    auto u = (i + randomDouble()) / (imageWidth - 1);
                    auto v = (j + randomDouble()) / (imageHeight - 1);
                    Ray r = cam.getRay(u, v);
                    pixelColor += rayColor(r, world, maxDepth);
                }

                pointers[0]->emplace_back(pixelColor);
            }
        }
    });

    // Bottom half
    std::thread t2([cam, world, pointers, imageHeight, imageWidth]() mutable {
        for (int j = (imageHeight / 2) - 1; j >= 0; --j) {
            for (int i = 0; i < imageWidth; ++i) {
                Color pixelColor(0, 0, 0);
                for (int s = 0; s < samplesPerPixel; ++s) {
                    auto u = (i + randomDouble()) / (imageWidth - 1);
                    auto v = (j + randomDouble()) / (imageHeight - 1);
                    Ray r = cam.getRay(u, v);
                    pixelColor += rayColor(r, world, maxDepth);
                }

                pointers[1]->emplace_back(pixelColor);
            }
        }
    });

    std::cerr << "\rLoading...";

    t1.join();
    t2.join();

    std::cout << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";

    for (auto pointer : pointers) {
        for (auto color = pointer->begin(); color != pointer->end(); ++color) {
            writeColor(std::cout, *color, samplesPerPixel);
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;

    std::cerr << "\nTask completed in " << duration.count() << "ms" << std::endl;
}
