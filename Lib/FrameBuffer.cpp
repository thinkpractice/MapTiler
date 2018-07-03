#include "FrameBuffer.h"

FrameBuffer::FrameBuffer()
{
	glGenFramebuffers(1, &_frameBufferId);
}

FrameBuffer::~FrameBuffer()
{
	glDeleteFramebuffers(1, &_frameBufferId);
}

void FrameBuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, _frameBufferId);
}

void FrameBuffer::Clear()
{
	glClearColor(0,0,0,1);
	glClear(GL_COLOR_BUFFER_BIT);
}