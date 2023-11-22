
// no matter which platform, we need this mesh
#ifndef MESH_H
#define MESH_H

#include "macros.h"


#if RENDER_ENABLED
#include <glad/glad.h>
#include "shader.h"
#endif

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <string>
#include <vector>
#include "collisionGeometry.h"



struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;

    Vertex()
    {
        Position = glm::vec3(0.0f);
        Normal = glm::vec3(0.0f);
        TexCoords = glm::vec2(0.0f);
    }

};



class Mesh
{
public:

    std::vector<Vertex> vertices;
    std::vector<unsigned int> textureIDs;
    unsigned int VAO;
    unsigned int VBO;

    Mesh(const std::string &filename, std::vector<unsigned int> textureIDs = std::vector<unsigned int>() );

    void addTexture(char const* path);

    void setMass(float mass);

    void calcInertiaTensor();

    collisionGeometry collision;

private:

    void setupMesh();

};















#endif //MESH_H
