#pragma once
#include "Test.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "FrameBuffer.h"
#include <memory>
namespace test
{
    class TestFrameBuffer :
        public Test
    {
	public:
        TestFrameBuffer();
        ~TestFrameBuffer();

        void OnUpdate(float deltatime) override;
        void OnRender() override;
		void OnImGuiRender() override;
		std::string GetTestName() override;
		void SetCamera(Camera& camera) override;
    private:
		Camera camera;
		std::unique_ptr<Shader> shader;
		std::unique_ptr<Shader> screenShader;
		//������
		std::unique_ptr<VertexArray> cubeVao;
		std::unique_ptr<IndexBuffer> cubeIbo;
		std::unique_ptr<VertexBuffer> cubeVb;
		std::unique_ptr<Texture> cubeTexuture;
		//�ذ�
		std::unique_ptr<VertexArray> planeVao;
		std::unique_ptr<VertexBuffer> planeVb;
		std::unique_ptr<Texture> planeTexuture;

		//֡����
		std::unique_ptr<FrameBuffer> frameBuffer;
		std::unique_ptr<Texture> quadTexture;

		std::unique_ptr<VertexArray> quadVao;
		std::unique_ptr<VertexBuffer> quadVb;
    };

}

