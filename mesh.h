
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

    collisionGeometry collision;

private:

    void setupMesh();

};















#endif //MESH_H
