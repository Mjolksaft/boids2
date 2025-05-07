#ifndef BOIDMANAGER_H
#define BOIDMANAGER_H

#include <vector>

#include "boid.h"

class BoidManager
{
public:
    BoidManager(int numBoids) : numBoids(numBoids)
    {
        // create the boids
        for (int i = 0; i < numBoids; i++)
        {
            boidList.emplace_back(
                Boid(
                    glm::vec3(Util::randomNumber(0.0f, 800.0f), Util::randomNumber(0.0f, 600.0f), 0.0f),
                    glm::vec3(Util::randomNumber(0.0f, 100.0f) / 100.0f, Util::randomNumber(0.0f, 100.0f) / 100.0f, Util::randomNumber(0.0f, 100.0f) / 100.0f),
                    glm::vec3(Util::randomNumber(0.1f, 100.0f)/100.0F, Util::randomNumber(0.1f, 100.0f)/100.0F, 0.0f)));
        }
        // add them to the list
    };

    void update()
    {
        for (int i = 0; i < numBoids; i++)
        {
            Boid &currentBoid = boidList[i];

            glm::vec3 closeDir = glm::vec3(0.0f, 0.0f, 0.0f);
            glm::vec3 velAvg = glm::vec3(0.0f, 0.0f, 0.0f);
            glm::vec3 avgPos = glm::vec3(0.0f, 0.0f, 0.0f);
            int neighboringBoids = 0;

            for (int j = 0; j < numBoids; j++)
            {
                if (i != j)
                {
                    Boid &otherBoid = boidList[j];
                    float dist = glm::length(otherBoid.getPosition() - currentBoid.getPosition());
                    // check if too close or in in visual range 
                    if (dist < 20.0f)
                    {
                        closeDir += currentBoid.getPosition() - otherBoid.getPosition();
                    } else if (dist < 50.0f)
                    {
                        velAvg += otherBoid.getVelocity();
                        avgPos += otherBoid.getPosition();
                        neighboringBoids++;
                    }
                }
            }
            if (neighboringBoids > 0)
            {
                glm::vec3 avgVel = velAvg / static_cast<float>(neighboringBoids);
                glm::vec3 alignment = (avgVel - currentBoid.getVelocity()) * 0.05f;
            
                glm::vec3 avgPosition = avgPos / static_cast<float>(neighboringBoids);
                glm::vec3 cohesion = (avgPosition - currentBoid.getPosition()) * 0.05f;
            
                currentBoid.addAcceleration(alignment + cohesion);
            }
            
            // Separation force
            if (glm::length(closeDir) > 0.0f)
            {
                glm::vec3 separation = glm::normalize(closeDir) * 0.05f;
                currentBoid.addAcceleration(separation);
            }
        }






        // seperation();
        // alignment();
        // cohesion();
    }



    void seperation()
    {
        for (int i = 0; i < numBoids; i++)
        {
            Boid &currentBoid = boidList[i];
            glm::vec3 closeDir = glm::vec3(0.0f, 0.0f, 0.0f);
            for (int j = 0; j < numBoids; j++)
            {
                if (i != j)
                {
                    Boid otherBoid = boidList[j];
                    float dist = glm::length(otherBoid.getPosition() - currentBoid.getPosition());
                    if (dist < 20.0f)
                    {
                        closeDir += currentBoid.getPosition() - otherBoid.getPosition();
                    }
                }
            }
            currentBoid.addAcceleration(closeDir * 0.005f);
        }
    }

    void alignment() {
        for (int i = 0; i < numBoids; i++)
        {
            glm::vec3 velAvg = glm::vec3(0.0f, 0.0f, 0.0f);;
            int neighboringBoids = 0;
            Boid &currentBoid = boidList[i];
            for (int j = 0; j < numBoids; j++)
            {
                if(i != j) {
                    Boid otherBoid = boidList[j];
                    float dist = glm::length(otherBoid.getPosition() - currentBoid.getPosition());
                    if (dist < 50.0f)
                    {
                        velAvg += otherBoid.getVelocity();
                        neighboringBoids += 1;
                    }
                }
                if (neighboringBoids > 0)
                {
                    velAvg /= neighboringBoids;
                    velAvg *= (velAvg - currentBoid.getVelocity());
                    velAvg *= 0.05;
                    currentBoid.addAcceleration(velAvg);
                }
            }
        }
    }

    void cohesion() {
        const float COHESION_RADIUS = 50.0f; // Radius to check for nearby boids
        const float COHESION_WEIGHT = 0.05f; // Weight for cohesion force
    
        for (int i = 0; i < numBoids; i++) {
            Boid& currentBoid = boidList[i]; // Use a reference to modify the actual boid
            glm::vec3 avgPos = glm::vec3(0.0f, 0.0f, 0.0f);
            int neighboringBoids = 0;
    
            // Loop through all other boids
            for (int j = 0; j < numBoids; j++) {
                if (i != j) {
                    Boid& otherBoid = boidList[j];
                    float dist = glm::length(otherBoid.getPosition() - currentBoid.getPosition());
                    if (dist < COHESION_RADIUS) {
                        avgPos += otherBoid.getPosition(); // Sum up positions of neighbors
                        neighboringBoids++;
                    }
                }
            }
    
            // Calculate and apply cohesion force
            if (neighboringBoids > 0) {
                avgPos /= static_cast<float>(neighboringBoids); // Average position
                glm::vec3 cohesionForce = avgPos - currentBoid.getPosition(); // Steering force
                currentBoid.addAcceleration(cohesionForce * COHESION_WEIGHT); // Apply weighted force
            }
        }
    }

    void render(const Shader &shaderProgram)
    {
        for (Boid &b : boidList)
        {
            b.update();
            b.render(shaderProgram);
        }
    };

private:
    std::vector<Boid> boidList;
    int numBoids;
    // Add private members here
};

#endif // BOIDMANAGER_H