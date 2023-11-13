#include "IndexBuffer.h"
#include "Render.h"
IndexBuffer::IndexBuffer():
	m_RenderID(0),
	m_Count(0)
{
}
IndexBuffer::IndexBuffer(const unsigned int* data,const int count) :
	m_Count(count)
{
	GLCALL(glGenBuffers(1, &this->m_RenderID));
	GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_RenderID));//绑定
	GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
	GLCALL(glDeleteBuffers(1, &this->m_RenderID));
}

void IndexBuffer::Bind() const
{
	GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_RenderID));//绑定

}

void IndexBuffer::UnBind() const
{
	GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));//绑定
}
