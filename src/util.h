#ifndef UTIL_H
#define UTIL_H

#include <cmath>
#include <vector>
#include <iostream>
#include <random>


// Utility functions
namespace Util {

    // Calculate the distance between two points in 2D space
    inline double distance(double x1, double y1, double x2, double y2) {
        return std::sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
    }

    // Normalize a 2D vector
    inline std::pair<double, double> normalize(double x, double y) {
        double length = std::sqrt(x * x + y * y);
        if (length == 0) return {0, 0};
        return {x / length, y / length};
    }

    // Generate a random 3D vector with components in the range [0, 1)
    inline float randomNumber(float min, float max) {
        std::random_device rd;  // Non-deterministic random seed
        std::mt19937 gen(rd()); // Mersenne Twister generator
        std::uniform_int_distribution<> dist(min, max); // Range: [1, 100]
    
        // Generate a random number
        int randomNumber = dist(gen);
        
        return randomNumber;
    }

}

#endif // UTIL_H