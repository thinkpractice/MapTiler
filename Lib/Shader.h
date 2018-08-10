#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#if defined(__APPLE__)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif


class Shader
{
public:
	Shader(GLenum shaderType, const char* shaderSource);
	virtual ~Shader();
	
	void Load();
	GLuint ShaderId() const;
	
private:
	bool _loaded;
	GLuint _shaderId;
	GLenum _shaderType;
	const char* _shaderSource;
};


#endif
