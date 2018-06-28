#include "Shader.h"
#include <iostream>

using namespace std;

Shader::Shader(GLenum shaderType, const char* shaderSource)
			:	_loaded(false),
				_shaderType(shaderType),
				_shaderSource(shaderSource)
{
}

Shader::~Shader()
{
	//TODO find out how to safely copy objects
	if (_loaded)
		glDeleteShader(_shaderId);
}

void Shader::Load()
{
	if (_loaded)
		return;
	
	_shaderId = glCreateShader(_shaderType);
	glShaderSource(_shaderId, 1, &_shaderSource, nullptr);
	glCompileShader(_shaderId);
	
	GLint status;
	glGetShaderiv(_shaderId, GL_COMPILE_STATUS, &status);
	
	if (status != GL_TRUE)
	{
		char buffer[512];
		glGetShaderInfoLog(_shaderId, 512, NULL, buffer);
		cout << "shader error: " << buffer << endl;
		return;
	}
	_loaded = true;
}

GLuint Shader::ShaderId() const
{
	return _shaderId;
}
