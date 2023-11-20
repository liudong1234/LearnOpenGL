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
	glGenTextures(1, &this->m_RenderID);
	glBindTexture(GL_TEXTURE_2D, this->m_RenderID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
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

Texture::Texture(std::vector<std::string>& faces, GLenum wrapS, GLenum wrapT):
	m_RenderID(0), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0), type("diffuse")
{
	glGenTextures(1, &this->m_RenderID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, this->m_RenderID);

	GLenum channel;
		stbi_set_flip_vertically_on_load(0);
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		this->m_LocalBuffer = stbi_load(faces[i].c_str(), &this->m_Width, &this->m_Height, &this->m_BPP, 0);
		if (this->m_LocalBuffer)
		{
			if (this->m_BPP == 1)
				channel = GL_RED;
			else if (this->m_BPP == 3)
				channel = GL_RGB;
			else if (this->m_BPP == 4)
				channel = GL_RGBA;
			else
				channel = GL_RGB;
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, channel, this->m_Width, this->m_Height, 0, channel, GL_UNSIGNED_BYTE, this->m_LocalBuffer
			);
			stbi_image_free(this->m_LocalBuffer);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(this->m_LocalBuffer);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, wrapS);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, wrapT);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
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

void Texture::CubeBind(unsigned int slot) const
{
	GLCALL(glBindTexture(GL_TEXTURE_CUBE_MAP, this->m_RenderID));
}

void Texture::UnBind() const
{
	GLCALL(glBindTexture(GL_TEXTURE_2D, 0));//解除绑定

}

void Texture::UnCubeBind() const
{
	GLCALL(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));//解除绑定
}

void Texture::SetType(std::string type)
{
	this->type = type;
}

void Texture::SetId(unsigned int id)
{
	this->m_RenderID = id;
}
