#pragma once

#include <glad/glad.h>

class FrameBuffer
{
public:
	FrameBuffer();


	void BindTexture2D(unsigned int textureId, GLenum target = GL_FRAMEBUFFER, unsigned int slot = 0);
	void Bind();
	void UnBind();
	~FrameBuffer();


private:
	unsigned int m_RenderID;
};

