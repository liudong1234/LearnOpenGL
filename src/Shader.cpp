#include <glad/glad.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "Shader.h"
#include "Render.h"

Shader::Shader():
	m_Filepath(""),
	m_RenderID(0)
{
}

Shader::Shader(const char* filepath) :
	m_Filepath(filepath), m_RenderID(0)
{
	ShaderProgramSource source = this->ParseShader(filepath);
	this->m_RenderID = this->CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader()
{
	GLCALL(glDeleteProgram(this->m_RenderID));
}

void Shader::Bind() const
{
	GLCALL(glUseProgram(this->m_RenderID));
}

void Shader::UnBind() const
{
	GLCALL(glUseProgram(0));
}

int Shader::GetUniformLocation(const std::string& name) const
{
	if (this->m_UniformLocationCache.find(name) != this->m_UniformLocationCache.end())
		return this->m_UniformLocationCache[name];
	int location = glGetUniformLocation(this->m_RenderID, name.c_str());
	if (location == -1)
	{
		glGetError();
		std::cout << "Warning: uniform " << name << " doesn't exist" << std::endl;

	}
	this->m_UniformLocationCache[name] = location;
	return location;
}

void Shader::SetUniformMat3f(const char* name, glm::mat3& matrix)
{
	GLCALL(glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

void Shader::SetUniformMat4f(const char* name, glm::mat4& matrix)
{

	GLCALL(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

void Shader::SetStructDirectionalLight(DirectionalLight& directionLight)
{
	this->SetUniform3f("directionLight.direction", directionLight.direction);
	this->SetUniform3f("directionLight.ambient", directionLight.ambient);
	this->SetUniform3f("directionLight.diffuse", directionLight.diffuse);
	this->SetUniform3f("directionLight.specular", directionLight.specular);
}

void Shader::SetStructPointLight(PointLight& pointLight)
{
	this->SetUniform3f("pointLight.position", pointLight.position);
	this->SetUniform3f("pointLight.ambient", pointLight.ambient);
	this->SetUniform3f("pointLight.diffuse", pointLight.diffuse);
	this->SetUniform3f("pointLight.specular", pointLight.specular);
	this->SetUniform1f("pointLight.constant", pointLight.constant);
	this->SetUniform1f("pointLight.linear", pointLight.linear);
	this->SetUniform1f("pointLight.quadratic", pointLight.quadratic);
}

void Shader::SetStructSpotLight(SpotLight& spotLight)
{
	this->SetUniform3f("spotLight.position", spotLight.position);
	this->SetUniform3f("spotLight.direction", spotLight.direction);
	this->SetUniform3f("spotLight.ambient", spotLight.ambient);
	this->SetUniform3f("spotLight.diffuse", spotLight.diffuse);
	this->SetUniform3f("spotLight.specular", spotLight.specular);
	this->SetUniform1f("spotLight.cutOff", spotLight.cutOff);
	this->SetUniform1f("spotLight.outerCutOff", spotLight.outerCutOff);
}

void Shader::SetStructPointLightArray(PointLight *pointLight, int size)
{
	this->SetUniform3f("pointLight[0].position", pointLight[0].position);
	this->SetUniform3f("pointLight[0].ambient", pointLight[0].ambient);
	this->SetUniform3f("pointLight[0].diffuse", pointLight[0].diffuse);
	this->SetUniform3f("pointLight[0].specular", pointLight[0].specular);
	this->SetUniform1f("pointLight[0].constant", pointLight[0].constant);
	this->SetUniform1f("pointLight[0].linear", pointLight[0].linear);
	this->SetUniform1f("pointLight[0].quadratic", pointLight[0].quadratic);

	this->SetUniform3f("pointLight[1].position", pointLight[1].position);
	this->SetUniform3f("pointLight[1].ambient",  pointLight[1].ambient);
	this->SetUniform3f("pointLight[1].diffuse",  pointLight[1].diffuse);
	this->SetUniform3f("pointLight[1].specular", pointLight[1].specular);
	this->SetUniform1f("pointLight[1].constant", pointLight[1].constant);
	this->SetUniform1f("pointLight[1].linear",   pointLight[1].linear);
	this->SetUniform1f("pointLight[1].quadratic",pointLight[1].quadratic);

	this->SetUniform3f("pointLight[2].position", pointLight[2].position);
	this->SetUniform3f("pointLight[2].ambient", pointLight[2].ambient);
	this->SetUniform3f("pointLight[2].diffuse", pointLight[2].diffuse);
	this->SetUniform3f("pointLight[2].specular", pointLight[2].specular);
	this->SetUniform1f("pointLight[2].constant", pointLight[2].constant);
	this->SetUniform1f("pointLight[2].linear", pointLight[2].linear);
	this->SetUniform1f("pointLight[2].quadratic", pointLight[2].quadratic);

	this->SetUniform3f("pointLight[3].position", pointLight[3].position);
	this->SetUniform3f("pointLight[3].ambient",  pointLight[3].ambient);
	this->SetUniform3f("pointLight[3].diffuse",  pointLight[3].diffuse);
	this->SetUniform3f("pointLight[3].specular", pointLight[3].specular);
	this->SetUniform1f("pointLight[3].constant", pointLight[3].constant);
	this->SetUniform1f("pointLight[3].linear",   pointLight[3].linear);
	this->SetUniform1f("pointLight[3].quadratic", pointLight[3].quadratic);
}

void Shader::SetUniform1i(const std::string& name, int value)
{
	GLCALL(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniform1f(const std::string& name, float value)
{
	GLCALL(glUniform1f(GetUniformLocation(name), value));
}

void Shader::SetUniform2f(const std::string& name, const glm::vec2& value)
{
	GLCALL(glUniform2f(GetUniformLocation(name), value.x, value.y));
}

void Shader::SetUniform3f(const std::string& name, const glm::vec3& value)
{
	GLCALL(glUniform3fv(GetUniformLocation(name), 1, &value[0]));
}

void Shader::SetUniform3f(const std::string& name, float v0, float v1, float v2)
{
	GLCALL(glUniform3f(GetUniformLocation(name), v0, v1, v2));
}

void Shader::SetUniform4f(const char* name, const glm::vec4& value)
{
	GLCALL(glUniform4f(GetUniformLocation(name), value.x, value.y, value.z, value.w));
}

void Shader::SetUniform4f(const char* name, float v0, float v1, float v2, float v3)
{
	GLCALL(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

ShaderProgramSource Shader::ParseShader(const char* filepath)
{
	std::ifstream stream(filepath);
	std::stringstream ss[2];
	std::string line;
	enum class ShaderType
	{
		NONE = -1,
		VERTEX = 0,
		FRAGMENT = 1
	};

	ShaderType type = ShaderType::NONE;
	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
				type = ShaderType::VERTEX;
			else if (line.find("fragment") != std::string::npos)
				type = ShaderType::FRAGMENT;
		}
		else
		{
			ss[int(type)] << line << "\n";
		}
	}

	return { ss[0].str(), ss[1].str() };
}


unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int program = glCreateProgram();
	//顶点着色器
	unsigned int vs = CompileShader(vertexShader, GL_VERTEX_SHADER);
	//片段着色器
	unsigned int fs = CompileShader(fragmentShader, GL_FRAGMENT_SHADER);

	glAttachShader(program, vs);
	glAttachShader(program, fs);

	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);
	return program;
}

unsigned int Shader::CompileShader(const std::string& source, unsigned int type)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	//判断是否有错误
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)_malloca(length * sizeof(char));//alloca在堆栈上分配内存
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "着色器错误提示：" << message << std::endl;
		glDeleteShader(id);
		return 0;
	}

	return id;
}