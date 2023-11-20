

#ifndef BODYINSTANCE_H
#define BODYINSTANCE_H


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

    EObjectType objectType;

    void setMovement(glm::vec3 momentum, glm::vec3 angularMomentum);
    void setMass(float mass);

    bodyInstance(EObjectType type, TransformComponent* transform, MovementComponent* movement, collisionGeometry* collision);
};









#endif //BODYINSTANCE_H
