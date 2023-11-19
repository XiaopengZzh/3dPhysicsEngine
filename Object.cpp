

#include "Object.h"
#include <iostream>


Object::Object()
{
    meshInstance = nullptr;
    objectType = EObjectType::STATIC;
    shaderInstance = Shader();
    location = glm::vec3(0.0f);
    rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
}


Object::Object(Mesh *mesh, EObjectType type, Shader shader)
{
    meshInstance = mesh;
    objectType = type;
    shaderInstance = shader;

    location = glm::vec3(0.0f);
    rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
}

void Object::setTransformation(glm::vec3 location, glm::quat rotation)
{
    this->location = location;
    this->rotation = rotation;
}

void Object::Draw(Camera &cam)
{
    if(shaderInstance.ID == 0)
        return;

    shaderInstance.use();
    shaderInstance.setVec3("viewPos", cam.Position);
    glm::mat4 projection = glm::perspective(glm::radians(cam.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    shaderInstance.setMat4("projection", projection);
    glm::mat4 view = cam.GetViewMatrix();
    shaderInstance.setMat4("view", view);


    for(unsigned int idx = 0; idx < meshInstance->textureIDs.size(); idx++)
    {
        glActiveTexture(GL_TEXTURE0 + idx);

        glBindTexture(GL_TEXTURE_2D, meshInstance->textureIDs[idx]);
    }

    glBindVertexArray(meshInstance->VAO);

    glm::mat4 model = glm::translate(glm::mat4(1.0f), location);

    if(rotation.w != 1)
    {
        model = glm::rotate(model, 2 * glm::acos(rotation.w), glm::vec3( rotation.x, rotation.y, rotation.z ));
    }

    //std::cout << 2 * glm::acos(rotation.w) << std::endl;

    /*
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            std::cout << model[i][j] << " ";
        }
        std::cout << std::endl;
        std::cout << "--------" << std::endl;
    }
    */

    shaderInstance.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, meshInstance->vertices.size());


}