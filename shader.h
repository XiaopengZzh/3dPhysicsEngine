#if defined(_WIN64) || defined(_WIN32) || defined(__APPLE__)
#ifndef SHADER_H

#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:

    // the shader program ID
    GLuint ID;

    Shader(const char* vertexPath, const char* fragmentPath);

    //activate the shader
    void use();

    // utility uniform functions
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;

};

#endif
#endif
