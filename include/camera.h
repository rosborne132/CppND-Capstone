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

            _origin = lookfrom;
            _horizontal = focusDist * viewportWidth * u;
            _vertical = focusDist * viewportHeight * v;
            _lowerLeftCorner = _origin - _horizontal / 2 - _vertical / 2 - focusDist * w;

            _lensRadius = aperture / 2;
        }

        Ray getRay(double s, double t) const {
            Vec3 rd = _lensRadius * randomInUnitDisk();
            Vec3 offset = _u * rd.x() + _v * rd.y();

            return Ray(
                _origin + offset,
                _lowerLeftCorner + s * _horizontal + t * _vertical - _origin - offset
            );
        }

    private:
        Point3 _origin;
        Point3 _lowerLeftCorner;
        Vec3 _horizontal;
        Vec3 _vertical;
        Vec3 _u, _v;
        double _lensRadius;
};

#endif