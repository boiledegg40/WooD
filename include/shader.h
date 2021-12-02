#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

class Shader
{
    public:
        Shader();

        GLuint shader;

        void use();
        void compile(const char* vertexPath, const char* fragmentPath);

        void set_bool(const std::string &name, bool value);
        void set_int(const std::string &name, int value);
        void set_float(const std::string &name, float value);
};