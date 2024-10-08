#include <string>
#include <iostream>
#include <gl/glew.h>
// docs.gl is a good doc website!
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include "errors.h"
#include "shader.h"
#include "Material.h"
#include "Light.h"
#include "Camera.h"

Shader::Shader(const std::string& vertexSource, const std::string& fragmentSource)
{
	m_RendererID = CreateShader(vertexSource, fragmentSource);
}

Shader::~Shader()
{
	GLCall(glDeleteProgram(m_RendererID));
}

void Shader::Bind() const
{
	GLCall(glUseProgram(m_RendererID));
}

void Shader::SetLocation(const GLchar* name)
{
	GLCall(m_location = glGetUniformLocation(m_RendererID, name));
}


int Shader::GetLocation(const std::string& name) const
{
	GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
	if (location == -1)
		std::cerr << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;
	return location;
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3) const
{
	// get the uniform variable from the shader in the form of its id, and then 
	// we can set the value by passing the id and the values we wish to use
	GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
	GLCall(glUniform4f(location, v0, v1, v2, v3));
}

void Shader::SetUniform4f(const std::string& name, const glm::vec4& vector) const
{
	// get the uniform variable from the shader in the form of its id, and then 
	// we can set the value by passing the id and the values we wish to use
	GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
	GLCall(glUniform4fv(location, 1, glm::value_ptr(vector)));
}

void Shader::SetUniform3f(const std::string& name, float v0, float v1, float v2) const
{
	// get the uniform variable from the shader in the form of its id, and then 
	// we can set the value by passing the id and the values we wish to use
	GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
	GLCall(glUniform3f(location, v0, v1, v2));
}

void Shader::SetUniform3f(const std::string& name, const glm::vec3& vector) const
{
	// get the uniform variable from the shader in the form of its id, and then 
	// we can set the value by passing the id and the values we wish to use
	GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
	GLCall(glUniform3fv(location, 1, glm::value_ptr(vector)));
}

void Shader::setUniformMat4(const std::string& name, const glm::mat4& matrix) const
{
	GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
	GLCall(glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix)));
}

void Shader::setUniformInt(const std::string& name, const int i) const
{
	GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
	GLCall(glUniform1i(location, i));
}

void Shader::setUniformFloat(const std::string& name, const float f) const
{
	GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
	GLCall(glUniform1f(location, f));
}


/*
5unsigned int type - openGL types are normally unsigned ints so we use this rather than the type to decouple from opengl
*/
unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
	GLCall(unsigned int id = glCreateShader(type));
	const char* src = source.c_str(); // pointer to the start of the source string
	// id for the shader
	// number of source codes
	// pointer to pointer to string
	// if length is null, it is assumed the string is null terminated
	GLCall(glShaderSource(id, 1, &src, nullptr));
	GLCall(glCompileShader(id));

	// error handeling
	int result;
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE)
	{
		int length;
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
		char* message = (char*)alloca(length * sizeof(char));
		GLCall(glGetShaderInfoLog(id, length, &length, message));

		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader" << std::endl;
		std::cout << message << std::endl;
		GLCall(glDeleteShader(id));
		return 0;
	}

	return id;
}


/* We take in the sorce code for the shaders as strings.*/
unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	// openGL requires the source code for the shader to compile, and it returns an id back for accssing the function
	GLCall(unsigned int program = glCreateProgram());

	GLCall(unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader));
	GLCall(unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader));

	//two files, need to link them 
	GLCall(glAttachShader(program, vs));
	GLCall(glAttachShader(program, fs));
	GLCall(glLinkProgram(program));
	GLCall(glValidateProgram(program));

	//we have linked the shaders stored in a program, so we can delete the shader.
	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));

	return program;
}

void LightingShader::SetUniforms(const glm::mat4 Model, const Camera camera, const Material material, const Light light) {
	this->Bind();
	this->SetUniform3f("u_material.ambient", material.ambient);
	this->SetUniform3f("u_material.diffuse", material.diffuse);
	this->SetUniform3f("u_material.specular", material.specular);
	this->setUniformFloat("u_material.shininess", material.shininess);

	this->SetUniform3f("u_light.ambient", light.ambient);
	this->SetUniform3f("u_light.diffuse", light.diffuse);
	this->SetUniform3f("u_light.specular", light.specular);

	this->SetUniform3f("u_lightPos", light.lightPos);

	this->SetUniform3f("u_viewPos", camera.getPosition());
	this->setUniformMat4("u_view", camera.getViewMatrix());
	this->setUniformMat4("u_projection", camera.getProjectionMatrix());

	this->setUniformMat4("u_model", Model);
}