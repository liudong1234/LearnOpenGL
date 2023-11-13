#include "NewBufferLayout.h"

NewBufferLayout::NewBufferLayout():
	index(0),
	size(0),
	type(0),
	normalized(GL_FALSE),
	stride(0),
	pointer(nullptr)
{
}

NewBufferLayout::NewBufferLayout(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer)
{
	this->index = index;
	this->size = size;
	this->type = type;
	this->normalized = normalized;
	this->stride = stride;
	this->pointer = pointer;
}

void NewBufferLayout::SetVertexAttribPointer()
{
	glEnableVertexAttribArray(index);
	glVertexAttribPointer(index, size, type, normalized, stride, pointer);
}
