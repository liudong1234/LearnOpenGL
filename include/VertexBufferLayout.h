#pragma once
#include <glad/glad.h>
#include <vector>
#include "Render.h"
#include "StructSet.h"

struct VertexBufferElement
{
	unsigned int type;
	unsigned int count;
	unsigned char normalizd;
	static unsigned int GetSizeOfType(unsigned int type)
	{
		switch (type)
		{
		case GL_FLOAT:
			return 4;
		case GL_UNSIGNED_INT:
			return 4;
		case GL_UNSIGNED_BYTE:
			return 1;
		}
		ASSERT(false);
		return 0;
	}
};
//布局
class VertexBufferLayout
{
private:
	//...
	std::vector<VertexBufferElement> m_Elements;
	//偏移
	unsigned int m_Stride;
public:
	VertexBufferLayout() :
		m_Stride(0)
	{

	}
	//~VertexBufferLayout();

	template<typename T>
	void Push(unsigned int count)
	{
		static_assert(false, "haha");
	}

	inline std::vector<VertexBufferElement> GetElements() const
	{
		return this->m_Elements;
	}

	inline unsigned int GetStride() const
	{
		return this->m_Stride;
	}
	inline void ClearBuffer() { this->m_Elements.clear(); }
};

template<>
inline void VertexBufferLayout::Push<float>(unsigned int count)
{
	this->m_Elements.push_back({ GL_FLOAT, count, GL_FALSE });
	this->m_Stride += VertexBufferElement::GetSizeOfType(GL_FLOAT) * count;
}

template<>
inline void VertexBufferLayout::Push<unsigned int>(unsigned int count)
{
	this->m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
	this->m_Stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT) * count;
}

template<>
inline void VertexBufferLayout::Push<unsigned char>(unsigned int count)
{
	this->m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
	this->m_Stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE) * count;
}
