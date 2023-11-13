#include "Render.h"
#include <iostream>
void GlClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

bool GlLogCall(const char* function, const char* file, int line)
{
	unsigned int error;
	while (error = glGetError())
	{
		std::cout << "[opengl Error] " << error << std::endl
			<< function << " " << std::endl
			<< file << " " << ":" << line << " \n";
		return false;
	}
	return true;
}

void Render::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
	shader.Bind();//为gpu绑定着色器程序；
	va.Bind();	  //包含实际处理的数据
	ib.Bind();
	GLCALL(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}

void Render::Draw(const VertexArray& va, const Shader& shader, int first, unsigned int size) const
{
	shader.Bind();
	va.Bind();
	GLCALL(glDrawArrays(GL_TRIANGLES, first, size));

}

void Render::Clear() const
{
	glClear(GL_COLOR_BUFFER_BIT);
}
