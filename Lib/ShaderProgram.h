#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <vector>
#include <GL/glew.h>
#if defined(__APPLE__)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include "Shader.h"

using namespace std;

class ShaderProgram
{
public:
	ShaderProgram();
	virtual ~ShaderProgram();
	
	void AddShader(const Shader& shader);
	void Create();
	void Use();

	GLuint ShaderProgramId() const;
	
private:
    GLuint CreateShaderProgram();
	
private:
	bool _created;
	GLuint _shaderProgramId;
	vector<Shader> _shaders;
	
};

#endif
