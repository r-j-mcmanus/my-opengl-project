#pragma once

#include <string>
#include <iostream>
#include <gl/glew.h>
// docs.gl is a good doc website!
#include <glm/mat4x4.hpp>
#include <GLFW/glfw3.h>
#include "errors.h"

class Shader {
public:
    Shader(const std::string& vertexSource, const std::string& fragmentSource);

    ~Shader();

    void Bind() const;

    void SetLocation(const GLchar* name);

    int GetLocation(const std::string& name) const;

    void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3) const;
    void SetUniform4f(const std::string& name, const glm::vec4& vector) const;

    void SetUniform3f(const std::string& name, float v0, float v1, float v2) const;
    void SetUniform3f(const std::string& name, const glm::vec3& vector) const;

    void setUniformMat4(const std::string& name, const glm::mat4& matrix) const;

    void setUniformInt(const std::string& name, const int i) const;

private:
    unsigned int m_RendererID;
    int m_location;

    /*
    *unsigned int type - openGL types are normally unsigned ints so we use this rather than the type to decouple from opengl
    */
    static unsigned int CompileShader(unsigned int type, const std::string& source);


    /* We take in the sorce code for the shaders as strings.*/
    static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
};