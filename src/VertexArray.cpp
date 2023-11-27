#include "VertexArray.h"
#include "VertexBufferLayout.h"

VertexArray::VertexArray()
{
	GLCALL(glGenVertexArrays(1, &this->m_RenderID));
	this->Bind();
}

VertexArray::~VertexArray()
{
	GLCALL(glDeleteVertexArrays(1, &this->m_RenderID));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& vbLayout)
{
	this->Bind();
	vb.Bind();
	const auto elements = vbLayout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		GLCALL(glEnableVertexAttribArray(i));
		GLCALL(glVertexAttribPointer(i, element.count, element.type, element.normalizd, vbLayout.GetStride(), (void*)offset));
		//offset += vbLayout.GetStride();
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
}

void VertexArray::Bind() const
{
	GLCALL(glBindVertexArray(this->m_RenderID));
}

void VertexArray::UnBind() const
{
	GLCALL(glBindVertexArray(0));
}
