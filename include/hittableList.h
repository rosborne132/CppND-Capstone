#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"

#include <memory>
#include <vector>

class HittableList : public Hittable {
public:
    HittableList() {};
    explicit HittableList(std::shared_ptr<Hittable> object) { add(object); }

    void clear() { objects.clear(); }
    void add(std::shared_ptr<Hittable> object) { objects.emplace_back(std::move(object)); }

    virtual bool hit(const Ray& r, double tMin, double tMax, HitRecord& rec) const override;

private:
    std::vector<std::shared_ptr<Hittable>> objects;
};

bool HittableList::hit(const Ray& r, double tMin, double tMax, HitRecord& rec) const {
    HitRecord tempRec;
    bool hitAnything = false;
    auto closestSoFar = tMax;

    for (const auto& object : objects) {
        if (object->hit(r, tMin, closestSoFar, tempRec)) {
            hitAnything = true;
            closestSoFar = tempRec.t;
            rec = tempRec;
        }
    }

    return hitAnything;
}

#endif