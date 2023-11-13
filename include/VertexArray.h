#pragma once

//顶点数组操作

#include "VertexBuffer.h"
class VertexBufferLayout;
class VertexArray
{
private:
	unsigned int m_RenderID;
public:
	VertexArray();
	~VertexArray();
	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& vbLayout);
	//绑定顶点数组
	void Bind() const;
	void UnBind() const;

};