

#include "Object.h"
#include <iostream>


Object::Object()
{
    //meshInstance = nullptr; no viable overloaded =, it's already nullptr;
    objectType = EObjectType::STATIC;
    shaderInstance = Shader();
    location = glm::vec3(0.0f);
    rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
}


Object::Object(const std::shared_ptr<Mesh>& mesh, EObjectType type, Shader shader)
: meshInstance(mesh),
  objectType(type),
  shaderInstance(shader),
  location(glm::vec3(0.0f)),
  rotation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f))
{

}

void Object::setTransformation(glm::vec3 _location, glm::quat _rotation)
{
    location = _location;
    rotation = _rotation;
}

void Object::Draw(Camera &cam)
{
#if RENDER_ENABLED
    if(shaderInstance.ID == 0)
        return;

    shaderInstance.use();
    shaderInstance.setVec3("viewPos", cam.Position);
    glm::mat4 projection = glm::perspective(glm::radians(cam.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, ZNEAR, ZFAR);
    shaderInstance.setMat4("projection", projection);
    glm::mat4 view = cam.GetViewMatrix();
    shaderInstance.setMat4("view", view);

    std::shared_ptr<Mesh> sharedMeshInstance = meshInstance.lock();
    if(!sharedMeshInstance)
    {
        std::cout << "weak mesh pointer is nullptr" << std::endl;
        return;
    }

    for(unsigned int idx = 0; idx < sharedMeshInstance->textureIDs.size(); idx++)
    {
        glActiveTexture(GL_TEXTURE0 + idx);

        glBindTexture(GL_TEXTURE_2D, sharedMeshInstance->textureIDs[idx]);
    }

    glBindVertexArray(sharedMeshInstance->VAO);

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
    glDrawArrays(GL_TRIANGLES, 0, sharedMeshInstance->vertices.size());
#endif // RENDER_ENABLED

}