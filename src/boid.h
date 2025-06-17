#ifndef BOID_H
#define BOID_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>

#include "shader.h"
#include "util.h"

class Boid
{
public:
    Boid()
    {
        position = glm::vec3(Util::randomNumber(0.0f, 800.0f), Util::randomNumber(0.0f, 600.0f), 0);
        velocity = glm::vec3(0, 0, 0);
        acceleration = glm::vec3(Util::randomNegativeNumber(0.0f, 100.0f), Util::randomNegativeNumber(0.0f, 100.0f), 0);
    }

    void update()
    {

        position += velocity;
        velocity += acceleration;
        velocity = Util::limit(MAX_VELOCITY, velocity);

        if (position.x > 850.0f)
        {
            position.x = 0.0f;
        }
        if (position.x < -50.0f)
        {
            position.x = 800.0f;
        }
        if (position.y > 650.0f)
        {
            position.y = 0.0f;
        }
        if (position.y < -50.0f)
        {
            position.y = 600.0f;
        }

    };

    glm::vec3 align(std::vector<Boid> flock)
    {
        glm::vec3 steering = glm::vec3(0.0f, 0.0f, 0.0f);
        int perception = 100;
        int total = 0;

        for (Boid& other : flock)
        {
            float dist = glm::length(other.getPosition() - getPosition());
            if (&other != this && dist < perception)
            {
                steering += other.getVelocity();
                total += 1;
            }
        }
        if (total > 0) {
            steering /= total;
            steering = Util::setMag(MAX_VELOCITY, steering);
            
            steering -= getVelocity();
            steering = Util::limit(ALIGN_MAX_FORCE, steering);
        }
        return steering;
    };

    glm::vec3 cohesion(std::vector<Boid> flock)
    {
        glm::vec3 steering = glm::vec3(0.0f, 0.0f, 0.0f);
        int perception = 100;
        int total = 0;

        for (Boid& other : flock)
        {
            float dist = glm::length(other.getPosition() - getPosition());
            if (&other != this && dist < perception)
            {
                steering += other.getPosition();
                total += 1;
            }
        }
        if (total > 0) {
            steering /= total;
            steering -= getPosition();
            steering = Util::setMag(MAX_VELOCITY, steering);
            steering -= getVelocity();
            steering = Util::limit(COHESION_MAX_FORCE, steering);
        }
        return steering;
    };

    glm::vec3 seperation(std::vector<Boid> flock)
    {
        glm::vec3 steering = glm::vec3(0.0f, 0.0f, 0.0f);
        int perception = 50;
        int total = 0;

        for (Boid& other : flock)
        {
            float dist = glm::length(other.getPosition() - getPosition());
            if (&other != this && dist < perception && dist > 0.0f)
            {
                glm::vec3 diff = getPosition() - other.getPosition();
                diff /= dist;
                steering += diff;
                total += 1;
            }
        }
        if (total > 0) {
            steering /= total;
            steering = Util::setMag(MAX_VELOCITY, steering);
            steering -= getVelocity();
            steering = Util::limit(SEPERATION_MAX_FORCE, steering);
        }
        return steering;
    };

    void flock(std::vector<Boid> boids) {
        acceleration *= 0;
        auto alignment = this->align(boids);
        auto cohesion = this->cohesion(boids);
        auto seperation = this->seperation(boids);
        acceleration += seperation;
        acceleration += alignment;
        acceleration += cohesion;
    }

    void moveToTarget()
    {
        glm::vec3 diff = target - position;
        acceleration = Util::limit(MAX_ACCELERATION, diff);
        // std::cout << acceleration.x << ' ' << acceleration.y << std::endl;
    }

    void setNewTarget()
    {
        float distance = glm::length(target - position);
        // std::cout << distance << std::endl;
        if (distance < TARGET_THRESHOLD)
        {
            target = glm::vec3(Util::randomNumber(0.0f, 800.0f), Util::randomNumber(0.0f, 600.0f), 0.0f);
            // std::cout << target.x << ' ' << target.y << std::endl;
        }
    }

    void render(const Shader &shaderProgram)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, position);

        // Calculate rotation angle based on direction
        float angle = std::atan2(-velocity.x, velocity.y);
        model = glm::rotate(model, angle, glm::vec3(0, 0, 1)); // Rotate around the Z-axis

        // Scale the boid
        model = glm::scale(model, glm::vec3(width, height, 1.0f)); // Use 1.0f for Z-axis scaling

        // Pass the model matrix and color to the shader
        shaderProgram.setMat4("u_model", model);
        shaderProgram.setVec3("u_color", color);

        // Draw the boid
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }

    bool operator!=(const Boid &other) const
    {
        return this != &other;
    }

    // Getter and Setter for width and height
    float getWidth() const { return width; }
    void setWidth(float w) { width = w; }

    float getHeight() const { return height; }
    void setHeight(float h) { height = h; }

    glm::vec3 getPosition() const { return position; }
    void setPosition(glm::vec3 p) { position = p; }

    glm::vec3 getVelocity() const { return velocity; }
    void setVelocity(glm::vec3 v) { velocity = v; }

    glm::vec3 getAcceleration() const { return acceleration; }
    void setAcceleration(glm::vec3 a) { acceleration = a; }
    void addAcceleration(glm::vec3 a) { acceleration += a; }

private:
    const float MAX_VELOCITY = 1.f;
    const float MAX_ACCELERATION = 1.f;
    const float TARGET_THRESHOLD = 25.0f;
    const float ALIGN_MAX_FORCE = 0.01f;
    const float COHESION_MAX_FORCE = 0.01f;
    const float SEPERATION_MAX_FORCE = 0.01f;

    glm::vec3 position;
    glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 target = glm::vec3(200.0f, 200.0f, 0.0f);

    glm::vec3 acceleration;

    glm::vec3 color;
    float width = 10.0f;
    float height = 10.0f;
};

#endif // BOID_H

// void moveToCursor(glm::vec3 &v) {
//     target = v;
//     glm::vec3 diff = target - position;
//     glm::normalize(diff);
//     diff *= 0.2f;
//     acceleration = diff;
//     std::cout << acceleration.x << ' ' << acceleration.y << std::endl;
// }
