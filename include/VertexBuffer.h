#pragma once

//顶点缓存区操作

class VertexBuffer
{
private:
	unsigned int m_RenderID;
public:
	VertexBuffer();
	VertexBuffer(const void* data,const int size);
	~VertexBuffer();

	void Bind() const;
	void UnBind() const;
};

