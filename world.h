

#ifndef WORLD_H
#define WORLD_H

#include <memory>
#include <vector>
#include <iostream>
#include "Object.h"
#include "camera.h"


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

    std::vector<std::shared_ptr<Object>> ObjectsList;

    void Draw(Camera &cam);

private:
    World();
};





#endif //WORLD_H
