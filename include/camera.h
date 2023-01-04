#ifndef CAMERA_H
#define CAMERA_H

#include "constants.h"

class Camera {
    public:
        Camera(
            Point3 lookfrom,
            Point3 lookat,
            Vec3   vup,
            double vfov, // vertical field-of-view in degrees
            double aspectRatio,
            double aperture,
            double focusDist
        ) {
            auto theta = degreesToRadians(vfov);
            auto h = tan(theta / 2);
            auto viewportHeight = 2.0 * h;
            auto viewportWidth = aspectRatio * viewportHeight;

            auto w = unitVector(lookfrom - lookat);
            auto u = unitVector(cross(vup, w));
            auto v = cross(w, u);

            origin = lookfrom;
            horizontal = focusDist * viewportWidth * u;
            vertical = focusDist * viewportHeight * v;
            lowerLeftCorner = origin - horizontal / 2 - vertical / 2 - focusDist * w;

            lensRadius = aperture / 2;
        }

        Ray getRay(double s, double t) const {
            Vec3 rd = lensRadius * randomInUnitDisk();
            Vec3 offset = u * rd.x() + v * rd.y();

            return Ray(
                origin + offset,
                lowerLeftCorner + s * horizontal + t * vertical - origin - offset
            );
        }

    private:
        Point3 origin;
        Point3 lowerLeftCorner;
        Vec3 horizontal;
        Vec3 vertical;
        Vec3 u, v, w;
        double lensRadius;
};

#endif