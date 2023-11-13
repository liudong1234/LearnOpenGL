#pragma once
#include "glad/glad.h"
class NewBufferLayout
{
public:
	NewBufferLayout();
	NewBufferLayout(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer);

	void SetVertexAttribPointer();

private:
	GLuint index;
	GLint size;
	GLenum type;
	GLboolean normalized;
	GLsizei stride;
	const void* pointer;
};

