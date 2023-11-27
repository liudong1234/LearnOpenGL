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
    class TestAdvancedLore :
        public Test
    {
    public:
        TestAdvancedLore();
        ~TestAdvancedLore();
        void OnUpdate(float delta) override;
        void OnRender() override;
        void OnImGuiRender() override;
        void SetCamera(Camera& camera);
        std::string GetTestName() const;
    private:
        std::unique_ptr<Shader> cubeShader;
        std::unique_ptr<VertexBuffer> cubeVb;
        std::unique_ptr<VertexArray> cubeVao;

        std::unique_ptr<VertexBuffer> cubeUniformVb;

        std::unique_ptr<IndexBuffer> cubeIb;
        std::unique_ptr<Texture> cubeTexture;
        std::unique_ptr<Texture> cubeTexture2;

        std::vector<Vertex> vertices;

        Camera camera;
    };

}

