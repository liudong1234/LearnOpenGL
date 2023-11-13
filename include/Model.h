#pragma once
#include "Shader.h"
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model
{
public:
	Model() {}
	Model(const char* path);
	~Model();
	void Draw(Shader& shader);

private:
	//ģ������
	std::vector<Mesh> meshes;
	std::string directory;

	//����ȫ������֤������������
	std::vector<TextureStruct> texturesLoaded;
	
	//��������
	std::vector<TextureStruct> textureResult;
	//����
	//ģ�͵���
	void LoadModel(std::string path);
	//���ݵ���
	void ProcessNode(aiNode* node, const aiScene* scene);
	//��Assimp�����ݽ�����Mesh����
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	
	void LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};

