#ifndef SPHERE_H
#define SPHERE_H

#include "constants.h"
#include "hittable.h"
#include "material.h"

class Sphere : public Hittable {
    public:
        Sphere() {}
        Sphere(Point3 cen, double r, std::shared_ptr<Material> m) : _center(cen), _radius(r), _matPtr(m) {};

        virtual bool hit(const Ray& r, double tMin, double tMax, HitRecord& rec) const override;

    private:
        Point3 _center;
        double _radius;
        std::shared_ptr<Material> _matPtr;

};

bool Sphere::hit(const Ray& r, double tMin, double tMax, HitRecord& rec) const {
    Vec3 oc = r.origin() - _center;
    auto a = r.direction().lengthSquared();
    auto halfB = dot(oc, r.direction());
    auto c = oc.lengthSquared() - _radius * _radius;
    auto discriminant = halfB * halfB - a * c;

    if (discriminant < 0) return false;

    auto sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    auto root = (-halfB - sqrtd) / a;

    if (root < tMin || tMax < root) {
        root = (-halfB + sqrtd) / a;
        if (root < tMin || tMax < root)
            return false;
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    Vec3 outwardNormal = (rec.p - _center) / _radius;
    rec.setFaceNormal(r, outwardNormal);
    rec.matPtr = _matPtr;

    return true;
}

#endif