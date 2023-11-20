

#include "macros.h"

#include "mesh.h"
#include <fstream>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "stb_image.h"
#if RENDER_ENABLED
#include "glad.h"
#endif //RENDER_ENABLED

Mesh::Mesh(const std::string &filename, std::vector<unsigned int> textureIDs)
{
    this->textureIDs = textureIDs;

    std::ifstream in(filename);

    int ptNum;
    in >> ptNum;

    for(int idx = 0; idx < ptNum; idx++)
    {
        Vertex vec;

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

    int collisionVertNum;
    in >> collisionVertNum;

    for(int idx = 0; idx < collisionVertNum; idx++)
    {
        glm::vec3 vec;
        in >> vec[0] >> vec[1] >> vec[2];
        collision.collisionVertices.push_back(vec);
    }

    int elementBufferNum;
    in >> elementBufferNum;

    for(int idx = 0; idx < elementBufferNum; idx++)
    {
        elementBuffer buf;
        in >> buf.Indices[0] >> buf.Indices[1] >> buf.Indices[2];
        collision.elementBufferObject.push_back(buf);
    }

}

void Mesh::addTexture(char const* path)
{
#if RENDER_ENABLED
    unsigned int textureID;
    glGenTextures(1, &textureID);
    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format = GL_RED;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    textureIDs.push_back(textureID);
#endif //RENDER_ENABLED
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