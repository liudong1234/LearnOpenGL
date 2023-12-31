#pragma once
#include "Test.h"
#include "Model.h"

#include<memory>

namespace test
{

	class TestLoadingModel :
		public Test
	{
	public:
		TestLoadingModel();
		void OnUpdate(float deltatime) override;
		void OnRender() override;
		void OnImGuiRender() override;
		void SetCamera(Camera& camera) override;
		~TestLoadingModel();


	private:
		Model modelEx;
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<VertexBuffer> m_VB;

		std::unique_ptr<IndexBuffer> m_IB;
		std::unique_ptr<Shader> shader;

		//��պ�
		std::unique_ptr<Shader> skysboxShader;
		std::unique_ptr<VertexArray> skysboxVao;
		std::unique_ptr<VertexBuffer> skysboxVb;
		std::unique_ptr<Texture> skysboxTexture;

		glm::vec3 lightPos;
		Camera camera;
	};

}