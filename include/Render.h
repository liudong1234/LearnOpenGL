#pragma once
#include <glad/glad.h>
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
//断言，继续vstudio的断言宏
#define ASSERT(x) if (!x) __debugbreak();

#define GLCALL(x) GlClearError();\
	x;\
	ASSERT(GlLogCall(#x, __FILE__, __LINE__))

void GlClearError();
bool GlLogCall(const char* function, const char* file, int line);

class Render
{
public:
	void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
	void Draw(const VertexArray& va, const Shader& shader, int first, unsigned int size) const;
	void Clear() const;
};