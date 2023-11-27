#include "macros.h"

#if RENDER_ENABLED
//be sure to include glad before glfw3 and before
//other header files that require opengl
#include <glad/glad.h>
#include <glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "camera.h"
#include "renderInit.h"
extern Camera camera;
extern float deltaTime, lastFrame;
#endif

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <chrono>
#include "omp.h"
#include "world.h"
#include "modelInit.h"
#include "hardwareInfo.h"

extern glm::vec3 gravity;
extern glm::vec3 gravityDirections[6];
extern float time_broadphase;
extern float time_collisionResponse;
extern float time_narrowphase;
extern float time_integration;

// default input values
unsigned int thread_count = 24;
float totalRunTime = 60.0f;
unsigned int cubeNum = 150;
unsigned int tetraNum = 50;
float gravityReverseInterval = 15.0f;

int main(int argc, char* argv[])
{
    // input
    //===========================================================================//
    for(int idx = 1; idx < argc; idx++)
    {
        std::string arg = argv[idx];
        if(arg == "-thread_count")
            thread_count = strtol(argv[++idx], nullptr, 10);
        else if(arg == "-time")
            totalRunTime = strtof(argv[++idx], nullptr);
        else if(arg == "-cubeNum")
            cubeNum = strtol(argv[++idx], nullptr, 10);
        else if(arg == "-tetraNum")
            tetraNum = strtol(argv[++idx], nullptr, 10);
        else if(arg == "-gravityReverseInterval")
            gravityReverseInterval = strtof(argv[++idx], nullptr);
    }

    unsigned int logicalCores = 1;
    if(getLogicalProcessorsCount(logicalCores))
        thread_count = (thread_count > logicalCores ? logicalCores : thread_count);

    omp_set_num_threads(thread_count);

    printf("======================================================\n");
    printf("logical cores count : %u\n", logicalCores);
    printf("Thread count : %u\n", thread_count);
    printf("total run time : %f\n", totalRunTime);
    printf("number of spawned cubes : %d\n", cubeNum);
    printf("number of spawned tetrahedrons : %d\n", tetraNum);
    printf("interval to reverse gravity : %f\n", gravityReverseInterval);
    printf("======================================================\n");

    //===========================================================================//

#if RENDER_ENABLED
    GLFWwindow* window = renderInit();
    if(!window)
        return -1;
    printf("window created.\n");
#endif
    //===========================================================================//

    std::shared_ptr<World> world = World::GetWorldInstance();

    //===========================================================================//

    modelInit();

    // ==========================================================================//

    world->physicsRegistration();

    movementsInit();

    // =========================================================================//

    auto startTime = std::chrono::high_resolution_clock::now();
    auto currentTime = startTime;
    auto previousTime = startTime;
    //unsigned int elapsedTime_ms = 0;
    float elapsedTime = 0.0f;

    float dt = 0.0f;

    int gravityChangeTag = 1;

    bool bShouldClose = false;

    unsigned int totalFrameCount = 0;

    printf("simulation begins...\n");

    #pragma omp parallel
    {
        #pragma omp single
        {
            while ((!bShouldClose) && (elapsedTime < totalRunTime)) {
                currentTime = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - previousTime);
                unsigned int dt_ms = duration.count();
                dt = 0.001f * (dt_ms < 1 ? 1.0f : static_cast<float>(dt_ms));
                previousTime = currentTime;

                elapsedTime += dt;
                if (elapsedTime > gravityChangeTag * gravityReverseInterval) {
                    gravity = gravityDirections[gravityChangeTag] * GRAVITY_ACC;
                    gravityChangeTag++;
                }

                world->simulate(dt);

#if RENDER_ENABLED
                auto currentFrame = static_cast<float>(glfwGetTime());
                deltaTime = currentFrame - lastFrame;
                lastFrame = currentFrame;

                // Process Input
                processInput(window);

                glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                world->Draw(camera);

                // swap the color buffer used to render current iteration with the previous one and show it as output to screen
                glfwSwapBuffers(window);

                // Check if any events are triggered(like keyboard inputs), updates the window state, calls the corresponding functions
                glfwPollEvents();

                bShouldClose = glfwWindowShouldClose(window);
#endif

                totalFrameCount++;
            }
        }   // pragma omp single
    } // pragma omp parallel

    printf("simulation ends.\n");
    printf("======================================================\n");
    printf("frames number in total : %d\n", totalFrameCount);
    printf("fps : %f \n", float(totalFrameCount) / elapsedTime);
    printf("time consumed in integration : %f\n", time_integration);
    printf("time consumed in broad phase : %f\n", time_broadphase);
    printf("time consumed in narrow phase : %f\n", time_narrowphase);
    printf("time consumed in collision response : %f\n", time_collisionResponse);
    printf("======================================================\n");



#if RENDER_ENABLED
    glfwTerminate();
#endif
	return 0;
}
