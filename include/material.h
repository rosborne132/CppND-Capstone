#ifndef MATERIAL_H
#define MATERIAL_H

#include "constants.h"
#include "hittable.h"

struct HitRecord;

class Material {
    public:
        virtual bool scatter(const Ray& rIn, const HitRecord& rec, Color& attenuation, Ray& scattered) const = 0;
};

class Lambertian : public Material {
    public:
        Lambertian(const Color& a) : _albedo(a) {}

        virtual bool scatter(const Ray& rIn, const HitRecord& rec, Color& attenuation, Ray& scattered) const override {
            auto scatterDirection = rec.normal + randomUnitVector();

            // Catch degenerate scatter direction
            if (scatterDirection.nearZero()) {
                scatterDirection = rec.normal;
            }

            scattered = Ray(rec.p, scatterDirection);
            attenuation = _albedo;

            return true;
        }

    private:
        Color _albedo;
};

class Metal : public Material {
    public:
        Metal(const Color& a, double f) : _albedo(a), _fuzz(f < 1 ? f : 1) {}

        virtual bool scatter(const Ray& rIn, const HitRecord& rec, Color& attenuation, Ray& scattered) const override {
            Vec3 reflected = reflect(unitVector(rIn.direction()), rec.normal);
            scattered = Ray(rec.p, reflected + _fuzz * randomInUnitSphere());
            attenuation = _albedo;

            return dot(scattered.direction(), rec.normal) > 0;
        }

    private:
        Color _albedo;
        double _fuzz;
};

class Dielectric : public Material {
    public:
        Dielectric(double index_of_refraction) : _ir(index_of_refraction) {}

        virtual bool scatter(
            const Ray& rIn, const HitRecord& rec, Color& attenuation, Ray& scattered
        ) const override {
            attenuation = Color(1.0, 1.0, 1.0);
            double refractionRatio = rec.frontFace ? (1.0 / _ir) : _ir;
            Vec3 unitDirection = unitVector(rIn.direction());
            double cosTheta = fmin(dot(-unitDirection, rec.normal), 1.0);
            double sinTheta = sqrt(1.0 - cosTheta * cosTheta);
            bool cannotRefract = refractionRatio * sinTheta > 1.0;
            Vec3 direction = cannotRefract || reflectance(cosTheta, refractionRatio) > randomDouble()
                ? reflect(unitDirection, rec.normal)
                : refract(unitDirection, rec.normal, refractionRatio);

            scattered = Ray(rec.p, direction);

            return true;
        }

    private:
        double _ir; // Index of Refraction

        static double reflectance(double cosine, double refIdx) {
            // Use Schlick's approximation for reflectance.
            auto r0 = (1 - refIdx) / (1 + refIdx);
            r0 *= r0;
            return r0 + (1 - r0) * pow((1 - cosine), 5);
        }
};

#endif