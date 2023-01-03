#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"

struct HitRecord {
    Point3 p;
    Vec3 normal;
    double t;
    bool frontFace;

    inline void setFaceNormal(const Ray& r, const Vec3& outwardNormal) {
        frontFace = dot(r.direction(), outwardNormal) < 0;
        normal = frontFace ? outwardNormal : -outwardNormal;
    }
};

class Hittable {
    public:
        virtual bool hit(const Ray& r, double tMin, double tMax, HitRecord& red) const = 0;
};

#endif