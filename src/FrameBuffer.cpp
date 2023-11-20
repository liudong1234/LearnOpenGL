#include "FrameBuffer.h"
#include "Render.h"

FrameBuffer::FrameBuffer()
{
	glGenFramebuffers(1, &this->m_RenderID);
}

void FrameBuffer::BindTexture2D(unsigned int textureId, GLenum target, unsigned int slot)
{
	GLCALL(glFramebufferTexture2D(target, GL_COLOR_ATTACHMENT0 + slot, GL_TEXTURE_2D, textureId, 0));
}


void FrameBuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, this->m_RenderID);
}

void FrameBuffer::UnBind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

FrameBuffer::~FrameBuffer()
{

}
