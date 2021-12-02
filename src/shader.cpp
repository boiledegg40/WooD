#include "shader.h"
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader()
{
}

void Shader::compile(const char* vertexPath, const char* fragmentPath)
{
    const char* vertexSource; // 2 string variables for the source code
    const char* fragmentSource;
    std::string vertex, fragment;
    std::ifstream file; // ifstream to read from the 2 files

    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        file.open(vertexPath);
        while(file.good())
        {
            std::string line;
            getline(file, line);
            vertex.append(line + "\n");
        }
        vertexSource = vertex.c_str();

        file.close();
        file.clear();

        file.open(fragmentPath);
        while(file.good())
        {
            std::string line;
            getline(file, line);
            fragment.append(line + "\n");
        }
        fragmentSource = fragment.c_str();
        
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    // Compile vertex shader
    GLuint vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);

    int success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if(!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::printf("Error: Vertex shader compilation failed: %s\n", infoLog);
        return;
    }

        // Compile fragment shaders
    GLuint fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);

    shader = glCreateProgram();

    glAttachShader(shader, vertexShader);
    glAttachShader(shader, fragmentShader);
    glLinkProgram(shader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if(!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::printf("Error: Fragment shader compilation failed: %s\n", infoLog);
        return;
    }


    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(shader, 512, NULL, infoLog);
        std::printf("Error: Shader program linking failed: %s\n", infoLog);
        return;
    }

    glDeleteShader(fragmentShader);
    glDeleteShader(vertexShader);
    
}

void Shader::use()
{
    glUseProgram(shader);
}

void Shader::set_bool(const std::string &name, bool value)
{
    glUniform1i(glGetUniformLocation(shader, name.c_str()), (int)value); 
}

void Shader::set_int(const std::string &name, int value)
{
    glUniform1i(glGetUniformLocation(shader, name.c_str()), value);
}

void Shader::set_float(const std::string &name, float value)
{
    glUniform1f(glGetUniformLocation(shader, name.c_str()), value);
}