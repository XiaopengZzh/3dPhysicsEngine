

#include "macros.h"

#include "mesh.h"
#include <fstream>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


Mesh::Mesh(const std::string &filename, std::vector<Texture> textures)
{
    this->textures = textures;

    Vertex vec;

    std::ifstream in(filename);

    int ptNum;
    in >> ptNum;

    for(int idx = 0; idx < ptNum; idx++)
    {
        float x, y, z;

        in >> x >> y >> z;

        vec.Position = glm::vec3(x, y, z);

        in >> x >> y >> z;

        vec.Normal = glm::vec3(x, y, z);

        in >> x >> y;

        vec.TexCoords = glm::vec2(x, y);

        vertices.push_back(vec);
    }

    setupMesh();
}

void Mesh::setupMesh()
{
#if RENDER_ENABLED
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);


    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
#endif //RENDER_ENABLED
}