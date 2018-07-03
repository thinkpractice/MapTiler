#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <GL/glew.h>
#include <GL/gl.h>

class FrameBuffer
{
public:
	FrameBuffer();
	virtual ~FrameBuffer();
	
	void Bind();
	void Clear();
	
private:
	GLuint _frameBufferId;
};

#endif