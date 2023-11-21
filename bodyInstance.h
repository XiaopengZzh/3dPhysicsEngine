

#ifndef BODYINSTANCE_H
#define BODYINSTANCE_H

#include "macros.h"
#include "physicsComponent.h"
#include "glm/glm.hpp"
#include "collisionGeometry.h"
#include "physicsDefinition.h"


class bodyInstance
{
public:

    float mass;// in kilogram

    TransformComponent* transform;
    MovementComponent* movement;

    collisionGeometry* collision;

    glm::vec3 pendingLinearImpulse;
    glm::vec3 pendingAngularImpulse;

    EObjectType objectType;

    void setMovement(glm::vec3 momentum, glm::vec3 angularMomentum);
    void setMass(float mass);

    bodyInstance(EObjectType type, TransformComponent* transform, MovementComponent* movement, collisionGeometry* collision);

    void integration(float dt);



};



#endif //BODYINSTANCE_H
