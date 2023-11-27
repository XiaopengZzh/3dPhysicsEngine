#ifndef SHADER_H
#define SHADER_H
#include "macros.h"

#if RENDER_ENABLED
#include <glad/glad.h>
#endif

#include <fstream>
#include <sstream>
#include <iostream>

#include "glm/glm.hpp"

class Shader
{
public:

    // the shader program ID
    unsigned int ID;

    Shader();

#if RENDER_ENABLED
    Shader(const char* vertexPath, const char* fragmentPath);

    //activate the shader
    void use();

    // utility uniform functions
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setMat4(const std::string &name, const glm::mat4 &mat) const;
    void setVec3(const std::string &name, const glm::vec3 &value) const;
    void setVec3(const std::string &name, float x, float y, float z) const;
#endif
};


#endif
