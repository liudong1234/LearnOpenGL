#pragma once
#include "Test.h"
#include "Model.h"
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


	private:
		Model modelEx;
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<VertexBuffer> m_VB;

		std::unique_ptr<IndexBuffer> m_IB;
		std::unique_ptr<Shader> shader;
		std::unique_ptr<Shader> planeShader;
		std::unique_ptr<Texture> planeTex;
		Camera camera;
	};

}