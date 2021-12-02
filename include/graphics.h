#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

extern GLFWwindow* window;

void init_window();

void process_input();

void display();
