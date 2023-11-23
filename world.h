

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
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"
#include "topology.h"

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

    void integration(float dt);

    bool narrowCheck(unsigned int idx1, unsigned int idx2, glm::vec3& minimalTranslationVector);

    glm::vec3 support(glm::vec3 dir, unsigned int idx);

    glm::vec3 EPA(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d, unsigned int idx1, unsigned int idx2);

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
