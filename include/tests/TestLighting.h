#pragma once
#include "Test.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"

#include <memory>

namespace test
{
    class TestLighting :
        public Test
    {
	public:
		TestLighting();
		~TestLighting();
		void OnUpdate(float deltatime) override;
		void OnRender() override;
		void OnImGuiRender() override;
		Camera GetCamera() override;
		void SetCamera(Camera& camera) override;
	private:
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<IndexBuffer> m_IB;
		std::unique_ptr<Shader> shader;
		std::unique_ptr<Shader> lightShader;
		std::unique_ptr<Texture> texture;
		std::unique_ptr<Texture> texture2;
		std::unique_ptr<Texture> texture3;
		std::unique_ptr<VertexBuffer> m_VB;
		std::unique_ptr<VertexArray> m_LightVAO;
		glm::vec3 lightPos;

		//glm::vec3 cameraPos;
		//glm::vec3 cameraFront;
		//glm::vec3 cameraUp;


		glm::vec3 lightColor;
		Camera camera;
    };

}

