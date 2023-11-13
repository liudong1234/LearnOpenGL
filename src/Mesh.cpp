#include "Mesh.h"
#include "NewBufferLayout.h"
#include "Render.h"
#include <string>

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<TextureStruct> vTexture):
	vertices(vertices),
	indices(indices),
	textures(vTexture)
{
	this->m_VAO = std::make_unique<VertexArray>();
	this->m_IB = std::make_unique<IndexBuffer>(&indices[0], indices.size());

	this->m_VAO->Bind();
	this->m_VB = std::make_unique<VertexBuffer>(&vertices[0], vertices.size() * sizeof(Vertex));

	std::vector<NewBufferLayout> layouts;
	layouts.push_back(NewBufferLayout(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0));
	layouts.push_back(NewBufferLayout(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal)));
	layouts.push_back(NewBufferLayout(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords)));
	
	layouts.push_back(NewBufferLayout(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent)));
	layouts.push_back(NewBufferLayout(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent)));
	layouts.push_back(NewBufferLayout(5, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_BoneIDs)));
	layouts.push_back(NewBufferLayout(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_Weights)));

	
	for (NewBufferLayout layout : layouts)
	{
		layout.SetVertexAttribPointer();
	}

	this->m_VAO->UnBind();

}

Mesh::~Mesh()
{
}

void Mesh::Draw(Shader& shader)
{
	//可能需要修改
	shader.Bind();
	Render render;
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;
	for (unsigned int i = 0; i < this->textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i); // 在绑定之前激活相应的纹理单元
		std::string number;
		std::string name = this->textures[i].type;
		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++);
		else if (name == "texture_normal")
			number = std::to_string(normalNr++);
		else if (name == "texture_height")
			number = std::to_string(heightNr++);
		GLCALL(shader.SetUniform1i(("material." + name + number).c_str(), i));

		GLCALL(glBindTexture(GL_TEXTURE_2D, textures[i].id));
	}

	//绘制网格
	render.Draw(*this->m_VAO, *this->m_IB, shader);
	glActiveTexture(GL_TEXTURE0);

}

Mesh::Mesh(Mesh&& other) noexcept:
	m_VAO(std::move(other.m_VAO)),
	m_VB(std::move(other.m_VB)),
	m_IB(std::move(other.m_IB))
{
	this->vertices = std::move(other.vertices);
	this->indices =  std::move(other.indices);
	this->textures = std::move(other.textures);
}

void Mesh::SetMesh()
{

}
