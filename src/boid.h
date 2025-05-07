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
    Boid(const glm::vec3 position, const glm::vec3 color)
        : position(position), color(color), acceleration(glm::vec3(0.0f, 0.0f, 0.0f)) {} // Constructor

    void update()
    {
        setNewTarget();
        moveToTarget();

        velocity += acceleration;
        velocity = limit(MAX_VELOCITY, velocity);
        position += velocity;
    }; // Update the boid's state

    glm::vec3 limit(float maxSpeed, glm::vec3 &v)
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

    void moveToTarget()
    {
        glm::vec3 diff = target - position;
        acceleration = limit(MAX_ACCELERATION, diff);
        // std::cout << acceleration.x << ' ' << acceleration.y << std::endl;
    }

    void setNewTarget()
    {
        float distance = glm::length(target - position);
        std::cout << distance << std::endl;
        if (distance < TARGET_THRESHOLD)
        {
            target = glm::vec3(Util::randomNumber(0.0f, 800.0f), Util::randomNumber(0.0f, 600.0f), 0.0f);
            std::cout << target.x << ' ' << target.y << std::endl;
        }
    }

    void draw(const Shader &shaderProgram)
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

    // Getter and Setter for width and height
    float getWidth() const { return width; }
    void setWidth(float w) { width = w; }

    float getHeight() const { return height; }
    void setHeight(float h) { height = h; }

    glm::vec3 getPosition() const { return position; }
    void setPosition(glm::vec3 p) { position = p; }

    glm::vec3 getVelocity() const { return velocity; }
    void setVelocity(glm::vec3 v) { velocity = v; }

private:
    const float MAX_VELOCITY = 1.0f;
    const float MAX_ACCELERATION = 0.02f;
    const float TARGET_THRESHOLD = 25.0f;

    glm::vec3 position;
    glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 target = glm::vec3(200.0f, 200.0f, 0.0f);

    glm::vec3 acceleration;

    glm::vec3 color;
    float width = 50.0f;
    float height = 50.0f;
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
