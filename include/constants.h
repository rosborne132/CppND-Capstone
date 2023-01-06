#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cmath>
#include <limits>
#include <memory>
#include <random>


// Usings
using std::shared_ptr;
using std::make_shared;
using std::sqrt;


// Constants
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;


// Utility Functions
inline double degreesToRadians(double degrees) { return degrees * pi / 180.0; }

inline double randomDouble() { return rand() / (RAND_MAX + 1.0); }

inline double randomDouble(double min, double max) { return min + (max - min) * randomDouble(); }

inline double randomSmallDouble() {
    std::mt19937 rng;
    rng.seed(std::random_device{}());
    std::uniform_real_distribution<double> dist(-3.0, 3.0);

    return dist(rng);
};

inline double clamp(double x, double min, double max) {
    if (x < min) return min;
    if (x > max) return max;

    return x;
}


// Common Headers
#include "ray.h"
#include "vec3.h"

#endif