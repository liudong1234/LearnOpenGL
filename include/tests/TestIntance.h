#pragma once
#include "Test.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Texture.h"
#include "Camera.h"
#include "StructSet.h"
#include <string>
#include <memory>
namespace test
{
    class TestIntance :
        public Test
    {
    public:
        TestIntance();
        ~TestIntance();
        void OnUpdate(float delta) override;
        void OnRender() override;
        void OnImGuiRender() override;
        void SetCamera(Camera& camera);
        std::string GetTestName() const;
    private:
        std::unique_ptr<Shader> quadShader;
        std::unique_ptr<VertexArray> quadVao;
        std::unique_ptr<VertexBuffer> quadVb;
        std::unique_ptr<VertexBuffer> instanceVbo;

        Camera camera;
    };

}

