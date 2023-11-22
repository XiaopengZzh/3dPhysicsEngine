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
#include "world.h"

#include <chrono>


extern Camera camera;
extern float deltaTime, lastFrame;

int main(int argc, char* argv[])
{
    GLFWwindow* window = renderInit();
    if(!window)
        return -1;

    //========================//
    std::shared_ptr<World> world = World::GetWorldInstance();

    //=======================================================================================//

    Shader CubeShader("../shaders/lightcasters.vs", "../shaders/lightcasters.fs");
    CubeShader.use();
    CubeShader.setInt("material.diffuse", 0);
    CubeShader.setInt("material.specular", 1);

    Shader tetraShader("../shaders/tetrahedron.vs", "../shaders/tetrahedron.fs");
    tetraShader.use();
    tetraShader.setInt("diffuse", 0);


    //=======================================================================================//

    std::shared_ptr<Mesh> cubeMesh = std::make_shared<Mesh>("../models/cube.xyz");
    //Mesh cube("../models/cube.xyz");
    cubeMesh->addTexture("../textures/container.png");
    cubeMesh->addTexture("../textures/container_specular.png");

    std::shared_ptr<Mesh> tetrahedronMesh = std::make_shared<Mesh>("../models/tetrahedron.xyz");
    //Mesh tetrahedron("../models/tetrahedron.xyz");
    tetrahedronMesh->addTexture("../textures/tetra.jpg");




    //Object cubeList[10];
    for(int idx = 0; idx < 10; idx++)
    {
        //cubeList[idx] = Object(&cube, EObjectType::DYNAMIC, CubeShader);
        world->CreateObject(cubeMesh, EObjectType::DYNAMIC, CubeShader);
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
        world->ObjectsList[idx].setTransformation(cubePositions[idx], cubeRotations[idx]);
    }

    //=================//
    //Object tetraList[3];

    for(int idx = 0; idx < 3; idx++)
    {
        //tetraList[idx] = Object(&tetrahedron, EObjectType::DYNAMIC, tetraShader);
        world->CreateObject(tetrahedronMesh, EObjectType::DYNAMIC, tetraShader);
    }

    glm::vec3 tetraPositions[3] = {
            glm::vec3(3.0f, 4.0f, 5.0f),
            glm::vec3(-3.0f, 1.0f, -8.0f),
            glm::vec3(4.0f, -6.0f, 4.0f)
    };

    glm::quat tetraRotation[3] = {
            glm::quat(0.2f, 1.0f, 0.0f, 0.0f),
            glm::quat(0.2f, 0.0f, 1.0f, 0.0f),
            glm::quat(0.3f, 0.0f, 0.0f, 1.0f)
    };

    for(int i = 0; i < 3; i++)
    {
        world->ObjectsList[i + 10].setTransformation(tetraPositions[i], tetraRotation[i]);
    }


    world->physicsRegistration();

    // set initial velocity
    for(auto &move : world->movements)
    {
        //body.setMovement(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f));
        move.momentum = glm::vec3(0.0f, 0.0f, 20.0f);
        move.angularMomentum = glm::vec3(0.0f);
    }

    // =================================//

    //int TotalFrame = 0;

    auto previousTime = std::chrono::high_resolution_clock::now();
    auto currentTime = previousTime;
    float dt = 0.0f;
    // render loop. Keep drawing images and handling user input until the program has been explicitly told to stop.
    while(!glfwWindowShouldClose(window))
    {

        //TotalFrame++;
        /*
        if(TotalFrame % 1000 == 0)
        {
            std::cout << "total frame : " << TotalFrame << std::endl;
            endTime = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
            std::cout << "duration : " << duration.count() << "ms" << std::endl;
        }
         */

        currentTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - previousTime);
        unsigned int dt_ms = duration.count();
        dt = 0.001f * (dt_ms < 1 ? 1.0f : static_cast<float>(dt_ms));

        previousTime = currentTime;

        world->simulate(dt);


        auto currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;


        // Process Input
        processInput(window);

        // render commands here.

        // A state-setting function
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        // we can specify which buffer we would like to clear. The possible bits are GL_COLOR_BUFFER, GL_DEPTH_BUFFER_BIT
        // and GL_STENCIL_BUFFER_BIT
        // A state-using function
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        world->Draw(camera);


        // swap the color buffer used to render current iteration with the previous one and show it as output to screen
        glfwSwapBuffers(window);

        // Check if any events are triggered(like keyboard inputs), updates the window state, calls the corresponding functions
        glfwPollEvents();
    }

    // de-allocate all resources
    glDeleteVertexArrays(1, &(cubeMesh->VAO));
    //glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &(cubeMesh->VBO));
    glDeleteProgram(CubeShader.ID);
    //glDeleteProgram(BulbShader.ID);

    // clean all of GLFW's resources that were allocated before.
    glfwTerminate();
	return 0;
}
