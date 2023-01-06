#include <iostream>
#include <chrono>
#include <future>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

#include "camera.h"
#include "constants.h"
#include "color.h"
#include "hittableList.h"
#include "material.h"
#include "sphere.h"

std::mutex mtx;

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

HittableList random_scene(int numOfSphere) {
    HittableList world;

    auto groundMaterial = std::make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
    world.add(std::make_shared<Sphere>(Point3(0, -1000, 0), 1000, groundMaterial));

    auto makeSphere = [&world](int a) {
        auto chooseMat = randomDouble();
        double x = randomSmallDouble();
        double y = 0.2;
        double z = randomSmallDouble();
        Point3 center(x, y, z);

        double sphereSize = 0.2;

        if ((center - Point3(4, 0.2, 0)).length() > 0.9) {
            std::shared_ptr<Material> sphereMaterial;

            if (chooseMat < 0.8) {
                // diffuse
                auto albedo = Color::random() * Color::random();
                sphereMaterial = std::make_shared<Lambertian>(albedo);
            } else if (chooseMat < 0.95) {
                // metal
                auto albedo = Color::random(0.5, 1);
                auto fuzz = randomDouble(0, 0.5);
                sphereMaterial = std::make_shared<Metal>(albedo, fuzz);
            } else {
                // glass
                sphereMaterial = std::make_shared<Dielectric>(1.5);
            }

            std::lock_guard<std::mutex> lock(mtx);
            world.add(std::make_shared<Sphere>(center, sphereSize, sphereMaterial));
        }
    };

    std::vector<std::future<void>> futures;

    for (int a = 0; a < numOfSphere; ++a) {
        futures.emplace_back(std::async(std::launch::async, makeSphere, a));
    }

    std::for_each(futures.begin(), futures.end(), [](std::future<void> &ftr) {
        ftr.wait();
    });

    return world;
}

int main() {
    int numOfSpheres;
    std::cerr << "Enter the number of spheres you would like to render: ";
    std::cin >> numOfSpheres;

    // Image
    const auto aspectRatio = 3.0 / 2.0;
    const int imageWidth = 1200;
    const int imageHeight = static_cast<int>(imageWidth / aspectRatio);
    const int samplesPerPixel = 50;
    const int maxDepth = 50;

    // Camera
    Point3 lookfrom(13, 2, 3);
    Point3 lookat(0, 0, 0);
    Vec3 vup(0, 1, 0);
    auto distToFocus = 10.0;
    auto aperture = 0.1;

    Camera cam(lookfrom, lookat, vup, 20, aspectRatio, aperture, distToFocus);

    auto start = std::chrono::high_resolution_clock::now();

    std::cerr << "\rLoading...";

    // World
    auto world = random_scene(numOfSpheres);

    // Render
    std::vector<std::thread> threads;

    std::shared_ptr<std::vector<Color>> pointers[4] = {
        std::make_shared<std::vector<Color>>(),
        std::make_shared<std::vector<Color>>(),
        std::make_shared<std::vector<Color>>(),
        std::make_shared<std::vector<Color>>()
    };

    int chunks[4][2] = {
        {imageHeight - 1, (imageHeight / 2) + (imageHeight / 4)},
        {(imageHeight / 2) + (imageHeight / 4) - 1, (imageHeight / 2)},
        {(imageHeight / 2) - 1, (imageHeight / 4)},
        {(imageHeight / 4) - 1, 0}
    };

    auto processPixels = [cam, world, imageHeight, imageWidth](int* range, std::shared_ptr<std::vector<Color>> colors) mutable {
        for (int j = range[0]; j >= range[1]; --j) {
            for (int i = 0; i < imageWidth; ++i) {
                Color pixelColor(0, 0, 0);
                for (int s = 0; s < samplesPerPixel; ++s) {
                    auto u = (i + randomDouble()) / (imageWidth - 1);
                    auto v = (j + randomDouble()) / (imageHeight - 1);
                    Ray r = cam.getRay(u, v);
                    pixelColor += rayColor(r, world, maxDepth);
                }

                colors->emplace_back(pixelColor);
            }
        }
    };

    for (size_t i = 0; i < 4; ++i) {
        threads.emplace_back(std::thread(processPixels, chunks[i], pointers[i]));
    }

    for (auto &t : threads) {
        t.join();
    }

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
