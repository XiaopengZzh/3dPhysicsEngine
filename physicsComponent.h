

#ifndef PHYSICSCOMPONENT_H
#define PHYSICSCOMPONENT_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

struct TransformComponent
{
    glm::vec3 position;
    glm::quat rotation;

    TransformComponent() : position(glm::vec3(0.0f)), rotation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f)){}
    TransformComponent(glm::vec3 pos, glm::quat rot) : position(pos), rotation(rot){}
};

struct MovementComponent
{
public:
    glm::vec3 momentum;
    glm::vec3 angularMomentum;

    MovementComponent() : momentum(glm::vec3(0.0f)), angularMomentum(glm::vec3(0.0f)){}
    MovementComponent(glm::vec3 mom, glm::vec3 angm) : momentum(mom), angularMomentum(angm){}

};





#endif //PHYSICSCOMPONENT_H
