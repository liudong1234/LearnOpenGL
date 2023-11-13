#include "VertexBuffer.h"
#include "Render.h"

VertexBuffer::VertexBuffer():
	m_RenderID(0)
{
}

VertexBuffer::VertexBuffer(const void* data, const int size)
{
	glGenBuffers(1, &this->m_RenderID);
	glBindBuffer(GL_ARRAY_BUFFER, this->m_RenderID);//绑定
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer()
{
	GLCALL(glDeleteBuffers(1, &this->m_RenderID));
}

void VertexBuffer::Bind() const
{
	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, this->m_RenderID));//绑定

}

void VertexBuffer::UnBind() const
{
	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));//绑定

}
