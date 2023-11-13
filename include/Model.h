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
	//模型数据
	std::vector<Mesh> meshes;
	std::string directory;

	//保存全局纹理保证它的生命周期
	std::vector<TextureStruct> texturesLoaded;
	
	//最终纹理
	std::vector<TextureStruct> textureResult;
	//函数
	//模型导入
	void LoadModel(std::string path);
	//数据导入
	void ProcessNode(aiNode* node, const aiScene* scene);
	//将Assimp的数据解析到Mesh类中
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	
	void LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};

