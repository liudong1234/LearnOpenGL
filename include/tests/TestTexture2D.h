#pragma once
#include "Test.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"
#include "Texture.h"
#include "Camera.h"
#include <memory>

namespace test
{
	class TestTexture2D :
		public Test
	{
	public:
		TestTexture2D();
		void OnUpdate(float deltatime) override;
		void OnRender() override;
		void OnImGuiRender() override;
		std::string GetTestName() override;
		void SetCamera(Camera& camera) override;
		~TestTexture2D();
	private:
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<VertexArray> lightVAO;
		std::unique_ptr<IndexBuffer> m_IB;
		std::unique_ptr<Shader> shader;
		std::unique_ptr<Shader> lightShader;
		std::unique_ptr<Texture> texture;
		std::unique_ptr<Texture> texture2;
		std::unique_ptr<Texture> texture3;
		std::unique_ptr<VertexBuffer> m_VB;

		glm::mat4 m_Project;
		glm::mat4 m_View;
		glm::mat4 m_Model;
		

		glm::vec3 lightPos;
		glm::vec3 lightDirect;
		Camera camera;

		glm::vec4 backgroundColor;
		glm::vec3 spotlightAmbient;
		glm::vec3 spotlightDiffuse;
		glm::vec3 spotlightSpecular;

		float materialDiffuse;
		float materialSpecular;

		int selectedItem = 0;
	};
}



