
#ifndef OBJECT_H
#define OBJECT_H

#include "macros.h"
#include "mesh.h"
#include "shader.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "camera.h"


enum class EObjectType
{
    STATIC,
    DYNAMIC
};

class Object
{
public:

    Mesh* meshInstance;
    EObjectType objectType;
    Shader shaderInstance;

    glm::vec3 location;
    glm::quat rotation;

    Object();

    Object(Mesh* mesh, EObjectType type, Shader shader);

    void setTransformation(glm::vec3 location, glm::quat rotation);

    void Draw(Camera& cam);


};


#endif //OBJECT_H
