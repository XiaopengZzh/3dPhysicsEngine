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



// a callback function used to resize the viewport with respect to resize of the window by users.
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

//
void processInput(GLFWwindow* window)
{
    // If user has pressed the escape key, we set the window close flag and the next condition check of the main while
    // loop will then fail and the application closes.
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main(int argc, char* argv[])
{
    //Initialize GLFW
    glfwInit();
    //configure several options
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //for macOS
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Create a window, this window contains all the windowing data and is required by most of GLFW's other functions.
    GLFWwindow* window = glfwCreateWindow(800, 600, "RBDexperimental", nullptr, nullptr);
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

    //================================================================================//

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;

    unsigned char* data = stbi_load("../textures/floor.jpg", &width, &height, &nrChannels, 0);

    if(data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    //=======================================================================================//

    Shader ourShader("../shaders/simple.vs", "../shaders/simple.fs");

    //=======================================================================================//

    float vertices[] = {
            0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f,
            0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            -0.5f, 0.5f, 0.0f, 0.3f, 0.3, 0.0f, 0.0f, 1.0f
    };

    unsigned int indices[] = {
            0, 1, 3,
            1, 2, 3
    };


    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint VBObuf;
    glGenBuffers(1, &VBObuf);

    glBindBuffer(GL_ARRAY_BUFFER, VBObuf);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // set vertex attribute pointer
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);


    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    //=========================================================================================//

    glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
    vec = trans * vec;

    std::cout << vec.x << vec.y << vec.z << std::endl;



    // render loop. Keep drawing images and handling user input until the program has been explicitly told to stop.
    while(!glfwWindowShouldClose(window))
    {
        // Process Input
        processInput(window);

        // render commands here.

        // A state-setting function
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);;
        // we can specify which buffer we would like to clear. The possible bits are GL_COLOR_BUFFER, GL_DEPTH_BUFFER_BIT
        // and GL_STENCIL_BUFFER_BIT
        // A state-using function
        glClear(GL_COLOR_BUFFER_BIT);


        // use our shader program when we want to render an object.
        //glUseProgram(shaderProgramID);

        ourShader.use();
        //ourShader.setFloat()

        glBindTexture(GL_TEXTURE_2D, textureID);

        // when bind VAO, it automatically binds VBO, EBO, regenerate vertex attributes pointers for us.
        glBindVertexArray(vao);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);



        // swap the color buffer used to render current iteration with the previous one and show it as output to screen
        glfwSwapBuffers(window);

        // Check if any events are triggered(like keyboard inputs), updates the window state, calls the corresponding functions
        glfwPollEvents();
    }

    // de-allocate all resources
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &VBObuf);
    glDeleteProgram(ourShader.ID);

    // clean all of GLFW's resources that were allocated before.
    glfwTerminate();
	return 0;
}
