#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "boid.h"

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

Boid boid1(glm::vec3(400.0f, 300.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f));

int main() {
    std::cout << "Hello, World!" << std::endl;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "Hello World", nullptr, nullptr);
    
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "glad failed to load" << std::endl;
        return -1;
    }
    

    const char* vertex = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
    
        void main() 
        {
            gl_Position = vec4(aPos, 1.0);
        }
    )";
    
    const char* fragment = R"(
        #version 330 core
        out vec4 FragColor;
    
        void main() 
        {
            FragColor = vec4(0.0, 1.0, 0.0, 1.0);
        }
    )";
   
    float indices[] = {
        -0.5f, -0.5f, 0.0f, // bottom left 
        0.5f, -0.5f, 0.0f, // bottom right 
        0.0f, 0.5f, 0.0f // top mid
    };

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
    glEnableVertexAttribArray(0);


    Shader shaderProgram("C:/Users/kalla/git/test2/boids/shader/vertex.vert", "C:/Users/kalla/git/test2/boids/shader/fragment.frag");


    while(!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shaderProgram.use();


        glm::mat4 projection = glm::ortho(0.0f, 800.0f, 600.0f, 0.0f, -1.0f, 1.0f);
        shaderProgram.setMat4("u_projection", projection);

        glBindVertexArray(VAO);
        boid1.draw(shaderProgram);
        boid1.update();


        glfwSwapBuffers(window);
        glfwPollEvents(); 
    }
    
    glfwTerminate();

    
    return 0;
}


// glfwSetCursorPosCallback(window, cursor_position_callback); // for the mouse pos

// void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
//     glm::vec3 cursorPosition(xpos, ypos, 0.0f);
//     boid1.moveToCursor(cursorPosition);
//     std::cout << xpos << ' ' << ypos << std::endl;
// }