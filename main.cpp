#include <iostream>
//be sure to include glad before glfw3 and before
//other header files that require opengl
#include <glad/glad.h>
#include <glfw3.h>
#include "shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "macros.h"
#include "renderInit.h"

#include "camera.h"

#include "mesh.h"
#include "Object.h"

extern Camera camera;
extern float deltaTime, lastFrame;

int main(int argc, char* argv[])
{
    //Initialize GLFW
    glfwInit();
    //configure several options
    // Specify the major version and minor version of OpenGL, which is 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //for macOS
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Create a window, this window contains all the windowing data and is required by most of GLFW's other functions.
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "RBDexperimental", nullptr, nullptr);
    if(window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    //tell GLFW to make the context of our window the main context on the current thread.
    glfwMakeContextCurrent(window);


    // Initialize GLAD before we call any OpenGL function
    // We pass GLAD the function to load the address of the OpenGL function pointers which is OS-specific
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    /* The first two parameters set the location of the lower left corner of the window.
       The third and fourth set the width and height of the rendering window in pixels, which we will set equal to
       GLFW's window size.
       We could actually set the viewport dimensions at values smaller than GLFW's dimensions. In that way all the openGL
       rendering would be displayed in a smaller region of the window, and we could display other elements outside
       the region, like a game engine does.
    */
    glViewport(0, 0, 800, 600);

    // register callback function to window resize.
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glEnable(GL_DEPTH_TEST);

    //=======================================================================================//

    Shader CubeShader("../shaders/lightcasters.vs", "../shaders/lightcasters.fs");


    //=======================================================================================//

    Mesh cube("../models/cube.xyz");

    Object cubeList[10];
    for(int idx = 0; idx < 10; idx++)
    {
        cubeList[idx] = Object(&cube, EObjectType::DYNAMIC, CubeShader);
    }

    // world space positions of our cubes
    glm::vec3 cubePositions[] = {
            glm::vec3( 0.0f,  0.0f,  0.0f),
            glm::vec3( 2.0f,  5.0f, -15.0f),
            glm::vec3(-1.5f, -2.2f, -2.5f),
            glm::vec3(-3.8f, -2.0f, -12.3f),
            glm::vec3( 2.4f, -0.4f, -3.5f),
            glm::vec3(-1.7f,  3.0f, -7.5f),
            glm::vec3( 1.3f, -2.0f, -2.5f),
            glm::vec3( 1.5f,  2.0f, -2.5f),
            glm::vec3( 1.5f,  0.2f, -1.5f),
            glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    glm::quat cubeRotations[10];
    glm::vec3 pivot = glm::normalize(glm::vec3(1.0f, 0.3f, 0.5f));
    for(int idx = 0; idx < 10; idx++)
    {
        cubeRotations[idx] = glm::quat(glm::cos( glm::radians(20.0f * idx / 2.0f )), pivot);
    }

    for(int idx = 0; idx < 10; idx++)
    {
        cubeList[idx].setTransformation(cubePositions[idx], cubeRotations[idx]);
    }


    cube.addTexture("../textures/container.png");
    cube.addTexture("../textures/container_specular.png");


    CubeShader.use();
    CubeShader.setInt("material.diffuse", 0);
    CubeShader.setInt("material.specular", 1);


    // render loop. Keep drawing images and handling user input until the program has been explicitly told to stop.
    while(!glfwWindowShouldClose(window))
    {

        auto currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;


        // Process Input
        processInput(window);

        // render commands here.

        // A state-setting function
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);;
        // we can specify which buffer we would like to clear. The possible bits are GL_COLOR_BUFFER, GL_DEPTH_BUFFER_BIT
        // and GL_STENCIL_BUFFER_BIT
        // A state-using function
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        for(unsigned int i = 0; i < 10; i++)
        {
            cubeList[i].Draw(camera);
        }


        // swap the color buffer used to render current iteration with the previous one and show it as output to screen
        glfwSwapBuffers(window);

        // Check if any events are triggered(like keyboard inputs), updates the window state, calls the corresponding functions
        glfwPollEvents();
    }

    // de-allocate all resources
    glDeleteVertexArrays(1, &(cube.VAO));
    //glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &(cube.VBO));
    glDeleteProgram(CubeShader.ID);
    //glDeleteProgram(BulbShader.ID);

    // clean all of GLFW's resources that were allocated before.
    glfwTerminate();
	return 0;
}
