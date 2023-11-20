#pragma once
#include "Render.h"

#include <vector>
#include <string>
class Texture
{
private:
	unsigned int m_RenderID;
	std::string m_FilePath;
	unsigned char* m_LocalBuffer;
	int m_Width, m_Height, m_BPP;//宽高位深
	
	std::string type;
public:
	Texture();
	Texture(const std::string& path, GLenum wrapS= GL_CLAMP_TO_EDGE, GLenum wrapT= GL_CLAMP_TO_EDGE);
	Texture(std::vector<std::string>& faces, GLenum wrapS = GL_CLAMP_TO_EDGE, GLenum wrapT = GL_CLAMP_TO_EDGE);
	~Texture();
	void Bind(unsigned int slot = 0) const;
	void CubeBind(unsigned int slot = 0) const;
	void UnBind() const;
	void UnCubeBind() const;
	void SetType(std::string type);
	void SetId(unsigned int id);


	inline int GetWidth() const { return this->m_Width; }
	inline int GetHeight() const { return this->m_Height; }
	inline int GetBPP() const { return this->m_BPP; }
	inline std::string GetType() const { return this->type; }
	inline std::string GetFilePath() const { return this->m_FilePath; }
	inline int GetID() const { return this->m_RenderID; }
};