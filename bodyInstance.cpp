



#include "bodyInstance.h"


void bodyInstance::setMovement(glm::vec3 momentum, glm::vec3 angularMomentum)
{
    movement->momentum = momentum;
    movement->angularMomentum = angularMomentum;
}

void bodyInstance::setMass(float _mass)
{
    mass = _mass;
}


bodyInstance::bodyInstance(EObjectType type, TransformComponent *transform, MovementComponent *movement, collisionGeometry *collision)
{
    objectType = type;
    this->transform = transform;
    this->movement = movement;
    this->collision = collision;
    mass = 1.0f;
}
