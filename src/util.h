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
    inline int randomNumber(float min, float max) {
        std::random_device rd;  // Non-deterministic random seed
        std::mt19937 gen(rd()); // Mersenne Twister generator
        std::uniform_int_distribution<> dist(min, max); // Range: [1, 100]
    
        // Generate a random number
        int randomNumber = dist(gen);
        
        return randomNumber;
    }


        // Generate a random 3D vector with components in the range [0, 1)
    inline int randomNegativeNumber(float min, float max) {
        float negative = Util::randomNumber(0.f,1.f);
        float randomNumber = Util::randomNumber(min,max);
        
        if (negative < 0.5f) {
            randomNumber = randomNumber * -1;
        }

        return randomNumber;
    }


    inline glm::vec3 limit(float maxSpeed, glm::vec3 &v)
    {
        float mag = glm::length(v);
        if (mag == 0)
            return v;

        if (mag > maxSpeed)
        {
            v /= mag;
            v *= maxSpeed;
        }

        return v;
    }

    inline glm::vec3 setMag(float maxSpeed, glm::vec3 &v)
    {
        float mag = glm::length(v);
        if (mag == 0)
            return v;

        v /= mag;
        v *= maxSpeed;

        return v;
    }

}

#endif // UTIL_H