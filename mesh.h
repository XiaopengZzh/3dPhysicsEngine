
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
};


struct Texture
{
    unsigned int id;
    std::string type;
    std::string path;
};

class Mesh
{
public:

    std::vector<Vertex> vertices;
    std::vector<Texture> textures;
    unsigned int VAO;
    unsigned int VBO;

    Mesh(const std::string &filename, std::vector<Texture> textures = std::vector<Texture>());
    //void Draw(Shader &shader);

private:

    void setupMesh();

};















#endif //MESH_H
