



#include "bodyInstance.h"


void bodyInstance::setMovement(glm::vec3 momentum, glm::vec3 angularMomentum)
{
    //movement->momentum = momentum;
    //movement->angularMomentum = angularMomentum;
}

void bodyInstance::setMass(float _mass)
{
    mass = _mass;
}


bodyInstance::bodyInstance(EObjectType type, collisionGeometry *collision)
{
    objectType = type;
    //this->transform = transform;
    //this->movement = movement;
    this->collision = collision;
    mass = 1.0f;
    pendingLinearImpulse = glm::vec3(0.0f);
    pendingAngularImpulse = glm::vec3(0.0f);
}


void bodyInstance::integration(float dt)
{
    /*
    if(transform == nullptr || movement == nullptr) return;

    pendingLinearImpulse += mass * glm::vec3(0.0f, 0.0f, -GRAVITY_ACC) * dt;

    movement->momentum += pendingLinearImpulse;

    //todo movement->angularMomentum += pendingAngularImpulse
    */
}