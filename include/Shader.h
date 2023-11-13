#pragma once

#include "glm/glm.hpp"
#include "StructSet.h"
#include <string>
#include <unordered_map>

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader
{
private:
	const char* m_Filepath;
	unsigned int m_RenderID;
	mutable std::unordered_map<std::string, int> m_UniformLocationCache;
public:
	Shader();
	Shader(const char* filepath);
	~Shader();

	void Bind() const;
	void UnBind() const;
	//设置uniform,可以拓展为任意参数，重载
	void SetUniform1i(const std::string& name, int value);
	void SetUniform1f(const std::string& name, float value);
	void SetUniform2f(const std::string& name, const glm::vec2& value);
	void SetUniform3f(const std::string& name, const glm::vec3& value);
	void SetUniform3f(const std::string& name, float v0, float v1, float v2);
	void SetUniform4f(const char* name, float v0, float v1, float v2, float v3);
	void SetUniform4f(const char* name, const glm::vec4& value);

	void SetUniformMat3f(const char* name, glm::mat3& matrix);
	void SetUniformMat4f(const char* name, glm::mat4& matrix);

	//结构体uninform 光源
	void SetStructDirectionalLight(DirectionalLight& directionLight);
	void SetStructPointLight(PointLight& pointLight);
	void SetStructSpotLight(SpotLight& spotLight);

	void SetStructPointLightArray(PointLight* pointLight, int size);

private:
	ShaderProgramSource ParseShader(const char* filepath);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	unsigned int CompileShader(const std::string& source, unsigned int type);
	int GetUniformLocation(const std::string& name) const;
};

