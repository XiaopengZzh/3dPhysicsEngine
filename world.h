

#ifndef WORLD_H
#define WORLD_H



#include <memory>
#include <vector>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <chrono>
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
#include "AABB.h"

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

    void CreateObject(std::shared_ptr<Mesh> mesh, EObjectType type, Shader shader = Shader());

#if RENDER_ENABLED
    void Draw(Camera &cam);
#endif

    void initialAABBs();

    void initialValTags();

    void physicsRegistration();

    void simulate(float dt);

    void integration(float dt);

    pairlist broadPhase();

    void collisionResponse(collisionInfolist& collInfolist);

    collisionInfolist narrowPhase(pairlist& potentialCollidedPairs);

    // if collision happens, minimalTranslationVector is the direction that object idx1 should move towards.
    // Object idx2 should move along -minimalTranslationalVector
    bool narrowCheck(unsigned int idx1, unsigned int idx2, glm::vec3& minimalTranslationVector);

    glm::vec3 support(glm::vec3 dir, unsigned int idx);
    int supportRetDiff(glm::vec3 dir, unsigned int idx, float& diff);

    glm::vec3 EPA(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d, unsigned int idx1, unsigned int idx2);

    glm::vec3 calcContactPoint(glm::vec3 minimalTranslationalVector, unsigned int idx1, unsigned int idx2);

    void resolvePenetration(glm::vec3 minimalTranslationalVector, unsigned int idx1, unsigned int idx2);

    void collisionResponseInternal(glm::vec3 mtv, unsigned int idx1, unsigned int idx2, glm::vec3 contactPt);

    void resolveClipping();

    void syncTransform();

    void updateAABBs();

    void updateValTag();

    std::vector<Object> ObjectsList;
    std::vector<std::shared_ptr<Mesh>> meshList;

    // ECS, stores transform and movement information of all physics instance in one block of memory
    std::vector<TransformComponent> transforms;
    std::vector<MovementComponent> movements;

    std::vector<bodyInstance> bodyInstances;

    std::vector<AABB> AABBlist;
    std::vector<valTag> flagsX;
    std::vector<valTag> flagsY;
    std::vector<valTag> flagsZ;

private:

    World();

};





#endif //WORLD_H
