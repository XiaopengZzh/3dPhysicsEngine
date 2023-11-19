

#ifndef RENDERINIT_H
#define RENDERINIT_H

#include "macros.h"
#include <glad/glad.h>
#include <glfw3.h>
#include "camera.h"
#include <iostream>

GLFWwindow* renderInit();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xposIn, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);










#endif //RENDERINIT_H
