#include "FrameBuffer.h"

FrameBuffer::FrameBuffer(int width, int height, GLenum colorFormat)
                :	_width(width),
                    _height(height),
                    _colorFormat(colorFormat)
{
	glGenFramebuffers(1, &_frameBufferId);

}

FrameBuffer::~FrameBuffer()
{
    glDeleteTextures(1, &_textureId);
    glDeleteFramebuffers(1, &_frameBufferId);
}

bool FrameBuffer::StatusOk()
{
   return Status() == GL_FRAMEBUFFER_COMPLETE;
}

GLenum FrameBuffer::Status()
{
    return glCheckFramebufferStatus(GL_FRAMEBUFFER);
}

void FrameBuffer::Bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, _frameBufferId);
    glGenTextures(1, &_textureId);
    glBindTexture(GL_TEXTURE_2D, _textureId);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _width, _height, 0, _colorFormat, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _textureId, 0);
}

void FrameBuffer::Clear()
{
	glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT);
}

GLuint FrameBuffer::TextureId()
{
    return _textureId;
}

int FrameBuffer::Width() const
{
    return _width;
}

int FrameBuffer::Height() const
{
    return _height;
}

GLenum FrameBuffer::ColorFormat() const
{
    return _colorFormat;
}
