#pragma once

#include "glm/glm.hpp"
#include <string>
#define MAX_BONE_INFLUENCE 4
//����
struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
	//bone indexes which will influence this vertex
	int m_BoneIDs[MAX_BONE_INFLUENCE];
	//weights from each bone
	float m_Weights[MAX_BONE_INFLUENCE];
};

struct TextureStruct 
{
	unsigned int id;
	std::string type;
	std::string path;
};
//�����
struct DirectionalLight
{
	glm::vec3 direction;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};
//���Դ
struct PointLight
{
	glm::vec3 position;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	float constant;
	float linear;
	float quadratic;
};
//�۹�Դ
struct SpotLight
{
	glm::vec3 direction;
	glm::vec3 position;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float cutOff;//���й��(����cosֵ)
	float outerCutOff;//���й��(����cosֵ)
};