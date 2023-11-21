

#ifndef WORLD_H
#define WORLD_H



#include <memory>
#include <vector>
#include <iostream>
#include "macros.h"
#include "Object.h"
#include "camera.h"
#include "bodyInstance.h"
#include "mesh.h"


class World
{
public:

    static std::shared_ptr<World> GetWorldInstance()
    {
        static std::shared_ptr<World> instance(new World);

        return instance;
    }

    World(const World&) = delete;
    World& operator=(const World&) = delete;

    void Init();

    void CreateObject(std::shared_ptr<Mesh> mesh, EObjectType type, Shader shader);

    void Draw(Camera &cam);

    void physicsRegistration();

    void simulate(float dt);

    void syncTransform();


    std::vector<Object> ObjectsList;

    // ECS, stores transform and movement information of all physics instance in one block of memory
    std::vector<TransformComponent> transforms;
    std::vector<MovementComponent> movements;

    std::vector<bodyInstance> bodyInstances;

private:

    World();

};





#endif //WORLD_H
