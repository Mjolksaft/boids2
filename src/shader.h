#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <glad/glad.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
public:
    // Constructor reads and builds the shader
    Shader(const std::string& vertexPath, const std::string& fragmentPath) {
        std::ifstream vertexFile(vertexPath);
        std::ostringstream vertexBuffer;
        vertexBuffer << vertexFile.rdbuf();
        std::string vertexCode = vertexBuffer.str();

        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        const char* vertexCodeStr = vertexCode.c_str();
        glShaderSource(vertexShader, 1, &vertexCodeStr, NULL);

        std::ifstream fragmentFile(fragmentPath);
        std::ostringstream fragmentBuffer;
        fragmentBuffer << fragmentFile.rdbuf();
        std::string fragmentCode = fragmentBuffer.str();

        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        const char* fragmentCodeStr = fragmentCode.c_str();
        glShaderSource(fragmentShader, 1, &fragmentCodeStr, NULL);

        ID = glCreateProgram();
        glAttachShader(ID, vertexShader);
        glAttachShader(ID, fragmentShader);
    
        glLinkProgram(ID);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    };

    // Use the shader program
    void use() const {
        glUseProgram(ID);
    };

    // Utility functions to set uniforms
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setMat4(const std::string& name, const glm::mat4& value) const {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
    };
    void setVec3(const std::string& name, const glm::vec3& value) const {
        glUniform3f(glGetUniformLocation(ID, name.c_str()), value.x, value.y, value.z);
    };

private:
    // The program ID
    GLuint ID;

    // Helper function to compile shaders
    void checkCompileErrors(unsigned int shader, const std::string& type);
};

#endif // SHADER_H