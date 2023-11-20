
#include "world.h"


World::World()
{
    std::cout << "The world is created." << std::endl;
}

void World::CreateObject(std::shared_ptr<Mesh> mesh, EObjectType type, Shader shader)
{
    ObjectsList.push_back(std::make_shared<Object>(mesh, type, shader));
}

void World::Init()
{

}

void World::Draw(Camera &cam)
{
    for(int idx = 0; idx < ObjectsList.size(); idx++)
    {
        ObjectsList[idx]->Draw(cam);
    }
}