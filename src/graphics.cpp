#include <cstdio>
#include "graphics.h"

const char* vertexShaderSource = "#version 330 core\n" // Explicitly state that using 3.3 and core functionality
        "layout (location = 0) in vec3 aPos;\n" // Declare all vertex attributes with in keyword, only need single because only position data
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";

const char* fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

GLFWwindow* window;
GLuint shaderProgram;
GLuint VAO, VBO, EBO;

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

static void error_callback(int error, const char* description)
{
    std::fprintf(stderr, "Error: %s\n", description);
}

void process_input()
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

bool init_window()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwSetErrorCallback(error_callback);

    window = glfwCreateWindow(800, 600, "Test", NULL, NULL);
    if (window == NULL)
    {
        std::printf("Failed to create GLFW window\n");
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window);
 
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::printf("Failed to initialize GLAD\n");
        return false;
    } 
    
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glViewport(0, 0, 800, 600);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    GLuint vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int  success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if(!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::printf("Error: Vertex shader compilation failed: %s\n", infoLog);
        return false;
    }

    GLuint fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if(!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::printf("Error: Fragment shader compilation failed: %s\n", infoLog);
        return false;
    }

    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::printf("Error: Shader program linking failed: %s\n", infoLog);
        return false;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    float vertices[] = 
    {
        0.5f,  0.5f, 0.0f,  // top right
        0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left
    };

    GLuint indices[] = 
    {
        0, 1, 3,
        1, 2, 3
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO); // Generate buffer and put handle in VBO
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO); // Represent intent to use buffer for vertex data
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // Copy vertex data into the buffer

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    return true;
}

bool display()
{
    if (glfwWindowShouldClose(window))
    {
        return false;
    }
    // Input
    process_input();

    // Rendering
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // Check for events and swap buffers
    glfwPollEvents();
    glfwSwapBuffers(window);

    return true;
}