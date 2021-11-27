#include <cstdio>
#include "graphics.h"

const char* vertexShaderSource = "#version 330 core\n" // Explicitly state that using 3.3 and core functionality
        "layout (location = 0) in vec3 aPos;\n" // Declare all vertex attributes with in keyword, only need single because only position data
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";

const char* fragmentShaderOrangeSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

const char* fragmentShaderYellowSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
    "}\n\0";

GLFWwindow* window;
GLuint shaderOrange, shaderYellow;
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

    // Compile vertex shader
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
    
    // Compile fragment shaders
    GLuint fragmentOrangeShader;
    fragmentOrangeShader = glCreateShader(GL_FRAGMENT_SHADER);
    GLuint fragmentYellowShader;
    fragmentYellowShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(fragmentOrangeShader, 1, &fragmentShaderOrangeSource, NULL);
    glCompileShader(fragmentOrangeShader);
    glShaderSource(fragmentYellowShader, 1, &fragmentShaderYellowSource, NULL);
    glCompileShader(fragmentYellowShader);

    shaderOrange = glCreateProgram();
    shaderYellow = glCreateProgram();

    glAttachShader(shaderOrange, vertexShader);
    glAttachShader(shaderOrange, fragmentOrangeShader);
    glLinkProgram(shaderOrange);

    glAttachShader(shaderYellow, vertexShader);
    glAttachShader(shaderYellow, fragmentYellowShader);
    glLinkProgram(shaderYellow);

    glGetShaderiv(fragmentOrangeShader, GL_COMPILE_STATUS, &success);

    if(!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(fragmentOrangeShader, 512, NULL, infoLog);
        std::printf("Error: Fragment shader compilation failed: %s\n", infoLog);
        return false;
    }


    glGetProgramiv(shaderOrange, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(shaderOrange, 512, NULL, infoLog);
        std::printf("Error: Shader program linking failed: %s\n", infoLog);
        return false;
    }

    glGetShaderiv(fragmentYellowShader, GL_COMPILE_STATUS, &success);

    if(!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(fragmentYellowShader, 512, NULL, infoLog);
        std::printf("Error: Fragment shader compilation failed: %s\n", infoLog);
        return false;
    }


    glGetProgramiv(shaderYellow, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(shaderYellow, 512, NULL, infoLog);
        std::printf("Error: Shader program linking failed: %s\n", infoLog);
        return false;
    }

    glDeleteShader(fragmentOrangeShader);
    glDeleteShader(fragmentYellowShader);

    glDeleteShader(vertexShader);

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

    glUseProgram(shaderOrange);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(3 * sizeof(GLuint)));

    glUseProgram(shaderYellow);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)0);


    // Check for events and swap buffers
    glfwPollEvents();
    glfwSwapBuffers(window);

    return true;
}