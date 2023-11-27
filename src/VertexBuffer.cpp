#include "VertexBuffer.h"
#include "Render.h"

VertexBuffer::VertexBuffer():
	m_RenderID(0)
{
}

VertexBuffer::VertexBuffer(const void* data, const int size, GLenum type)
{
	glGenBuffers(1, &this->m_RenderID);
	glBindBuffer(type, this->m_RenderID);//绑定
	glBufferData(type, size, data, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer()
{
	GLCALL(glDeleteBuffers(1, &this->m_RenderID));
}

void VertexBuffer::Bind(GLenum type) const
{
	GLCALL(glBindBuffer(type, this->m_RenderID));//绑定
}

void VertexBuffer::UnBind(GLenum type) const
{
	GLCALL(glBindBuffer(type, 0));//绑定
}

void VertexBuffer::BindData(GLenum type, const int size) const
{
	GLCALL(glBindBufferRange(type, 0, this->m_RenderID, 0, size));
}
