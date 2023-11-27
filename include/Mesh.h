#pragma once

#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Texture.h"
#include <memory>
#include <vector>
class Mesh
{
public:
	//��������
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<TextureStruct> textures;
	//����
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<TextureStruct> vTexture);
	~Mesh();
	void Draw(Shader& shader);
	Mesh(Mesh&& other) noexcept;
	std::unique_ptr<VertexArray>& GetVertexArray() {return this->m_VAO;}
private:
	std::unique_ptr<VertexArray> m_VAO;
	std::unique_ptr<IndexBuffer> m_IB;
	std::unique_ptr<VertexBuffer> m_VB;
	std::unique_ptr<VertexArray> m_LightVAO;

	void SetMesh();
};