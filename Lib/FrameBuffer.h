#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <GL/glew.h>
#include <GL/gl.h>

class FrameBuffer
{
public:
    FrameBuffer(int _width, int _height, GLenum ColorFormat = GL_RGBA);
	virtual ~FrameBuffer();
	
    bool StatusOk();
    GLenum Status();

    void Bind();
	void Clear();

    GLuint TextureId();
	
    int Width() const;
    int Height() const;
    GLenum ColorFormat() const;

private:
    GLuint _frameBufferId;
    GLuint _textureId;
    int _width;
    int _height;
    GLenum _colorFormat;
};

#endif
