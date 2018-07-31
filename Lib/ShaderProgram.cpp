#include "ShaderProgram.h"

ShaderProgram::ShaderProgram()
				:	_created(false)
{

}

ShaderProgram::~ShaderProgram()
{
	//TODO find out how we can safely copy shader program objects
	if (_created)
		glDeleteProgram(_shaderProgramId);
}

void ShaderProgram::AddShader(const Shader& shader)
{
	_shaders.push_back(shader);
}

void ShaderProgram::Create()
{ 
    _shaderProgramId = CreateShaderProgram();
	for (auto& shader : _shaders)
	{
		shader.Load();
		glAttachShader(_shaderProgramId, shader.ShaderId());
	}
	
	glBindFragDataLocation(_shaderProgramId, 0, "outColor");
	glLinkProgram(_shaderProgramId);
	_created = true;
}

void ShaderProgram::Use()
{
	glUseProgram(_shaderProgramId);
}

GLuint ShaderProgram::ShaderProgramId() const
{
    return _shaderProgramId;
}

GLuint ShaderProgram::CreateShaderProgram()
{
    if (_created)
        glDeleteProgram(_shaderProgramId);
    _shaderProgramId = glCreateProgram();
    _created = true;
    return _shaderProgramId;
}

