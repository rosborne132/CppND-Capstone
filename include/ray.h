#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class Ray {
    public:
        Ray() {}
        Ray(const Point3& origin, const Vec3& direction)
        : origin_(origin), direction_(direction) {}

    Point3 at(double t) const { return origin_ + t * direction_; }
    Vec3 direction() const { return direction_; }
    Point3 origin() const { return origin_; }

    private:
        Point3 origin_;
        Vec3 direction_;
};

#endif