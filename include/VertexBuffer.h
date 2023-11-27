#pragma once

//顶点缓存区操作
#include <glad/glad.h>
class VertexBuffer
{
private:
	unsigned int m_RenderID;
public:
	VertexBuffer();
	VertexBuffer(const void* data,const int size, GLenum type = GL_ARRAY_BUFFER);
	~VertexBuffer();

	void Bind(GLenum type = GL_ARRAY_BUFFER) const;
	void UnBind(GLenum type = GL_ARRAY_BUFFER) const;
	
	void BindData(GLenum type = GL_UNIFORM_BUFFER, const int size = 0) const;

	inline unsigned int GetId() const { return this->m_RenderID; }
};

