
#ifndef OBJECT_H
#define OBJECT_H

#include "macros.h"
#include "mesh.h"
#include "shader.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

enum class EObjectType
{
    STATIC,
    DYNAMIC
};

class object
{
public:

    Mesh* meshInstance;
    EObjectType objectType;
    Shader shaderInstance;

    glm::vec3 location;
    glm::quat rotation;


    object(Mesh* mesh, EObjectType type, Shader shader);

    void Draw();


};









#endif //OBJECT_H
