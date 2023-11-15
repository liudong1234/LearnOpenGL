#pragma once
#include "Test.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"

#include <memory>
namespace test
{
    class TestBlend :
        public Test
    {
	public:
		TestBlend();
		void OnUpdate(float deltatime) override;
		void OnRender() override;
		void OnImGuiRender() override;
		std::string GetTestName() override;
		void SetCamera(Camera& camera) override;
		~TestBlend();
	private:
		Camera camera;
		//������
		std::unique_ptr<VertexArray> cubeVao;
		std::unique_ptr<IndexBuffer> cubeIbo;
		std::unique_ptr<VertexBuffer> cubeVb;
		std::unique_ptr<Shader> cubeShader;
		std::unique_ptr<Texture> cubeTexture;
		//window
		std::unique_ptr<VertexArray> windowVao;
		std::unique_ptr<IndexBuffer> windowIbo;
		std::unique_ptr<VertexBuffer> windowVb;
		std::unique_ptr<Shader> windowShader;
		std::unique_ptr<Texture> windowTexture;
    };

}

