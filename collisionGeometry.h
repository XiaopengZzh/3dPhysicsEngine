//
// Created by Xiaopeng Zhang on 11/20/23.
//

#ifndef RBD_EXPERIMENTAL_COLLISIONGEOMETRY_H
#define RBD_EXPERIMENTAL_COLLISIONGEOMETRY_H

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <vector>

struct elementBuffer
{
public:
    // The order of the three vertices and the outward direction satisfy the right-hand rule.
    unsigned int Indices[3];

    elementBuffer(){Indices[0] = 0; Indices[1] = 0; Indices[2] = 0;}
    elementBuffer(unsigned int x, unsigned int y, unsigned int z)
    {
        Indices[0] = x;
        Indices[1] = y;
        Indices[2] = z;
    }
};

struct collisionGeometry
{
    std::vector<glm::vec3> collisionVertices;
    std::vector<elementBuffer> elementBufferObject;
    float mass;
    glm::mat3 inertiaTensor;
    glm::mat3 inertiaTensorInv;
};





#endif //RBD_EXPERIMENTAL_COLLISIONGEOMETRY_H
