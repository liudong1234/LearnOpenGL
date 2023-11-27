#pragma once
#include "Test.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Model.h"
#include "Camera.h"
#include "StructSet.h"
#include <string>
#include <memory>

/**/

namespace test
{
    class Planet :
        public Test
    {
    public:
        Planet();
        ~Planet();
        void OnUpdate(float delta) override;
        void OnRender() override;
        void OnImGuiRender() override;
        void SetCamera(Camera& camera);
        std::string GetTestName() const;
    private:
        std::unique_ptr<Shader> planetShader;
        std::unique_ptr<Shader> advanceShader;
        std::unique_ptr<VertexBuffer> vb;

        std::vector<std::unique_ptr<VertexArray>> vVa;

        Model planet;
        Model satellite;
        Camera camera;

        glm::mat4* modelMatrices;
    };

}

