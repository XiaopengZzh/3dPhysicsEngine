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

    Shader boundingWallShader("../shaders/boundingWall.vs", "../shaders/boundingWall.fs");
    boundingWallShader.use();
    boundingWallShader.setInt("diffuse", 0);
    boundingWallShader.setFloat("texScale", 8);

    //=======================================================================================//

    std::shared_ptr<Mesh> cubeMesh = std::make_shared<Mesh>("../models/cube.xyz");
    //Mesh cube("../models/cube.xyz");
    cubeMesh->setMass(2.0f);
    cubeMesh->addTexture("../textures/container.png");
    cubeMesh->addTexture("../textures/container_specular.png");

    std::shared_ptr<Mesh> tetrahedronMesh = std::make_shared<Mesh>("../models/tetrahedron.xyz");
    //Mesh tetrahedron("../models/tetrahedron.xyz");
    tetrahedronMesh->setMass(1.0f);
    tetrahedronMesh->addTexture("../textures/tetra.jpg");

    std::shared_ptr<Mesh> boundingWallMesh = std::make_shared<Mesh>("../models/boundingWall.xyz");
    boundingWallMesh->setMass(1.0f);
    boundingWallMesh->addTexture("../textures/boundingWall.jpg");




    // ====================================================================
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
    glm::vec3 axis = glm::normalize(glm::vec3(1.0f, 0.3f, 0.5f));
    for(int idx = 0; idx < 10; idx++)
    {
        float angle = glm::radians(20.0f * idx);
        cubeRotations[idx] = glm::angleAxis(angle, axis);
    }

    for(int idx = 0; idx < 10; idx++)
    {
        world->ObjectsList[idx].setTransformation(cubePositions[idx], cubeRotations[idx]);
    }

    //======================================================================//

    for(int idx = 0; idx < 3; idx++)
    {
        //tetraList[idx] = Object(&tetrahedron, EObjectType::DYNAMIC, tetraShader);
        world->CreateObject(tetrahedronMesh, EObjectType::DYNAMIC, tetraShader);
    }

    glm::vec3 tetraPositions[3] = {
            glm::vec3(-0.6f, 0.0f, 0.0f),
            glm::vec3(-3.0f, 1.0f, -8.0f),
            glm::vec3(4.0f, -6.0f, 4.0f)
    };

    glm::quat tetraRotation[3] = {
            glm::quat(0.8f, 0.0f, 0.0f, 1.0f),
            glm::quat(0.2f, 0.0f, 1.0f, 0.0f),
            glm::quat(0.3f, 0.0f, 0.0f, 1.0f)
    };

    for(int i = 0; i < 3; i++)
    {
        glm::normalize(tetraRotation[i]);
        world->ObjectsList[i + 10].setTransformation(tetraPositions[i], tetraRotation[i]);
    }
    // ====================================================================//

    for(int idx = 0; idx < 6; idx++)
    {
        world->CreateObject(boundingWallMesh, EObjectType::STATIC, boundingWallShader);
    }

    glm::vec3 boundingWallPositions[6] = {
            glm::vec3(200.0f, 0.0f, 0.0f),
            glm::vec3(-200.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 200.0f, 0.0f),
            glm::vec3(0.0f, -200.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 200.0f),
            glm::vec3(0.0f, 0.0f, -200.0f)
    };

    glm::quat boundingWallRotations[6] = {
            glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
            glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
            glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
            glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
            glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
            glm::quat(1.0f, 0.0f, 0.0f, 0.0f)
    };

    for(int idx = 0; idx < 6; idx++)
    {
        world->ObjectsList[idx + 13].setTransformation(boundingWallPositions[idx], boundingWallRotations[idx]);
    }

    // =====================================================//

    world->physicsRegistration();

    // set initial velocity
    for(auto &move : world->movements)
    {
        //body.setMovement(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f));
        move.momentum = glm::vec3(0.0f, 0.0f, 0.0f);
        move.angularMomentum = glm::vec3(0.0f, 0.0f, 0.0f);
    }

    // =================================//

    /*
    int TotalFrame = 0;
    auto startTime = std::chrono::high_resolution_clock::now();
    auto endTime = startTime;
    */

    auto startTime = std::chrono::high_resolution_clock::now();
    auto currentTime = startTime;
    auto previousTime = startTime;
    //unsigned int elapsedTime_ms = 0;

    float dt = 0.0f;
    // render loop. Keep drawing images and handling user input until the program has been explicitly told to stop.
    while(!glfwWindowShouldClose(window))
    {

        currentTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - previousTime);
        unsigned int dt_ms = duration.count();
        dt = 0.001f * (dt_ms < 1 ? 1.0f : static_cast<float>(dt_ms));
        previousTime = currentTime;
        //elapsedTime_ms += (dt_ms < 1 ? 1 : dt_ms);


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
