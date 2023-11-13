#include "Texture.h"
#include "stb_image.h"
#include <iostream>
Texture::Texture():
	m_RenderID(0),
	m_FilePath(""),
	m_BPP(0),
	m_Height(0),
	m_Width(0),
	m_LocalBuffer(0),
	type("")
{

}


Texture::Texture(const std::string& path, GLenum wrapS, GLenum wrapT) :
	m_RenderID(0), m_FilePath(path), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0), type("diffuse")
{
	//上下翻转,opengl以左下角为（0， 0）点，相当于从下往上
	stbi_set_flip_vertically_on_load(1);
	this->m_LocalBuffer = stbi_load(path.c_str(), &this->m_Width, &this->m_Height, &this->m_BPP, 0);

	GLCALL(glGenTextures(1, &this->m_RenderID));
	GLCALL(glBindTexture(GL_TEXTURE_2D, this->m_RenderID));

	GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS));
	GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,wrapT));
	GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
	GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	if (this->m_LocalBuffer)
	{
		if (this->m_BPP == 3)
		{
			GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->m_Width, this->m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, this->m_LocalBuffer));
		}
		else if(this->m_BPP == 4)
		{
			GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->m_Width, this->m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, this->m_LocalBuffer));
		}
		else if (this->m_BPP == 1)
		{
			GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, this->m_Width, this->m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, this->m_LocalBuffer));
		}

		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "纹理加载失败\n";
		stbi_image_free(this->m_LocalBuffer);
	}
	this->UnBind();


	if (this->m_LocalBuffer)
		stbi_image_free(this->m_LocalBuffer);
}

Texture::~Texture()
{
	GLCALL(glDeleteTextures(1, &this->m_RenderID));
}


void Texture::Bind(unsigned int slot /*= 0*/) const
{
	GLCALL(glActiveTexture(GL_TEXTURE0 + slot));
	GLCALL(glBindTexture(GL_TEXTURE_2D, this->m_RenderID));
}

void Texture::UnBind() const
{
	GLCALL(glBindTexture(GL_TEXTURE_2D, 0));//解除绑定

}

void Texture::SetType(std::string type)
{
	this->type = type;
}

void Texture::SetId(unsigned int id)
{
	this->m_RenderID = id;
}
