
#include "world.h"


World::World()
{
    std::cout << "The world is created." << std::endl;
}

void World::CreateObject(std::shared_ptr<Mesh> mesh, EObjectType type, Shader shader)
{
    ObjectsList.emplace_back(mesh, type, shader);
}

void World::Init()
{

}

void World::Draw(Camera &cam)
{
    for(auto & idx : ObjectsList)
    {
        idx.Draw(cam);
    }
}

void World::physicsRegistration()
{
    auto size = ObjectsList.size();

    for(int idx = 0; idx < size; idx++)
    {
        transforms.emplace_back(ObjectsList[idx].location, ObjectsList[idx].rotation);
        movements.emplace_back(glm::vec3(0.0f), glm::vec3(0.0f));

        std::shared_ptr<Mesh> mesh = ObjectsList[idx].meshInstance.lock();

        if(!mesh)
        {
            std::cout << "weak ptr fails to point to mesh" << std::endl;
            continue;
        }

        bodyInstances.emplace_back(ObjectsList[idx].objectType, &(mesh->collision));

    }

}

void World::simulate(float dt)
{
    integration(dt);

#if RENDER_ENABLED
    syncTransform();
#endif //RENDER_ENABLED
}

void World::integration(float dt)
{
    int size = ObjectsList.size();
    for(int idx = 0; idx < size; idx++)
    {
        bodyInstances[idx].pendingLinearImpulse += bodyInstances[idx].mass * glm::vec3(0.0f, -GRAVITY_ACC, 0.0f) * dt;

        movements[idx].momentum += bodyInstances[idx].pendingLinearImpulse;

        transforms[idx].position += dt * (movements[idx].momentum * 2.0f - bodyInstances[idx].pendingLinearImpulse) / (4 * bodyInstances[idx].mass);

        // todo movement->angularMomentum += pendingAngularImpulse
    }
}




void World::syncTransform()
{
    for(int idx = 0; idx < ObjectsList.size(); idx++)
    {
        ObjectsList[idx].location = transforms[idx].position;
        ObjectsList[idx].rotation = transforms[idx].rotation;
    }
}







