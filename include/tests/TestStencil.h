#include "Test.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"
#include "TeXture.h"
#include "Camera.h"
#include <memory>

namespace test
{
    class TestStencil :
        public Test
    {
    public:
        TestStencil();
        void OnUpdate(float deltatime) override;
        void OnRender() override;
        void OnImGuiRender() override;
        std::string GetTestName() override;
        void SetCamera(Camera& camera) override;
        ~TestStencil();
    private:
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<VertexArray> planeVAO;
		std::unique_ptr<VertexArray> grassVAO;

		std::unique_ptr<IndexBuffer> m_IB;
		std::unique_ptr<Shader> shader;
		std::unique_ptr<Shader> edgeShader;
		std::unique_ptr<Shader> planeShader;
		std::unique_ptr<Shader> grassShader;

		std::unique_ptr<Texture> cubeTexture;
		std::unique_ptr<Texture> planeTexture;
		std::unique_ptr<Texture> grassTexture;
		std::unique_ptr<Texture> edgeTexture;
		
		std::unique_ptr<VertexBuffer> m_VB;
		std::unique_ptr<VertexBuffer> m_planeVB;
		std::unique_ptr<VertexBuffer> m_grassVB;

		glm::mat4 m_Project;
		glm::mat4 m_View;
		glm::mat4 m_Model;
		Camera camera;

		float angle;
	private:
		//内置渲染函数
		void RenderCube(Render& render);
		void RenderCubeEdge(Render& render);
		void RenderPlane(Render& render);
		void RenderGrass(Render& render);
    };
}
