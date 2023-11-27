

#include "modelInit.h"


extern unsigned int thread_count;
extern float totalRunTime;
extern unsigned int cubeNum;
extern unsigned int tetraNum;



void modelInit()
{
    printf("initializing models...\n");
    std::shared_ptr<World> world = World::GetWorldInstance();

#if RENDER_ENABLED
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
#endif
    //=======================================================================================//

    std::shared_ptr<Mesh> cubeMesh = std::make_shared<Mesh>("../models/cube.xyz");
    cubeMesh->setMass(2.0f);

    std::shared_ptr<Mesh> tetrahedronMesh = std::make_shared<Mesh>("../models/tetrahedron.xyz");
    tetrahedronMesh->setMass(1.0f);

    std::shared_ptr<Mesh> boundingWallMesh = std::make_shared<Mesh>("../models/boundingWall.xyz");
    boundingWallMesh->setMass(1000.0f);

    world->meshList.push_back(cubeMesh);
    world->meshList.push_back(tetrahedronMesh);
    world->meshList.push_back(boundingWallMesh);

#if RENDER_ENABLED
    cubeMesh->addTexture("../textures/container.png");
    cubeMesh->addTexture("../textures/container_specular.png");
    tetrahedronMesh->addTexture("../textures/tetra.jpg");
    boundingWallMesh->addTexture("../textures/boundingWall.jpg");
#endif

    // ====================================================================
    //Object cubeList[10];
    for(int idx = 0; idx < cubeNum; idx++)
    {
        //cubeList[idx] = Object(&cube, EObjectType::DYNAMIC, CubeShader);
#if RENDER_ENABLED
        world->CreateObject(cubeMesh, EObjectType::DYNAMIC, CubeShader);
#elif RENDER_DISABLED
        world->CreateObject(cubeMesh, EObjectType::DYNAMIC);
#endif
    }

    for(int idx = 0; idx < tetraNum; idx++)
    {
#if RENDER_ENABLED
        world->CreateObject(tetrahedronMesh, EObjectType::DYNAMIC, tetraShader);
#elif RENDER_DISABLED
        world->CreateObject(tetrahedronMesh, EObjectType::DYNAMIC);
#endif
    }

    std::ifstream locfile("../data/ManyPoints.txt");
    std::ifstream rotfile("../data/ManyRots.txt");

    for(unsigned int idx = 0; idx < (cubeNum + tetraNum); idx++)
    {
        float x, y, z;
        locfile >> x >> y >> z;
        glm::vec3 location(x, y, z);
        float w;
        rotfile >> w >> x >> y >> z;
        glm::vec3 axis(x, y, z);
        float angle = glm::radians(w);
        glm::quat rot = glm::angleAxis(angle, axis);
        world->ObjectsList[idx].setTransformation(location, rot);
    }


    // ====================================================================//

    for(int idx = 0; idx < 6; idx++)
    {
#if RENDER_ENABLED
        world->CreateObject(boundingWallMesh, EObjectType::STATIC, boundingWallShader);
#elif RENDER_DISABLED
        world->CreateObject(boundingWallMesh, EObjectType::STATIC);
#endif
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
        world->ObjectsList[idx + cubeNum + tetraNum].setTransformation(boundingWallPositions[idx], boundingWallRotations[idx]);
    }

}



void movementsInit()
{
    printf("initializing movements...\n");

    std::shared_ptr<World> world = World::GetWorldInstance();

    std::ifstream in("../data/ManyMovements.txt");

    for(unsigned int idx = 0; idx < (cubeNum + tetraNum); idx++)
    {
        float x, y, z;
        in >> x >> y >> z;

        world->movements[idx].momentum = glm::vec3(x, y, z);

        in >> x >> y >> z;

        world->movements[idx].angularMomentum = glm::vec3(x, y, z);
    }

    for(int idx = 0; idx < 6; idx++)
    {
        world->bodyInstances[idx + cubeNum + tetraNum].setRestitution(1.0f);
    }

}

