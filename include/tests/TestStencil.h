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
    std::unique_ptr<IndexBuffer> m_IB;
    std::unique_ptr<Shader> shader;
    std::unique_ptr<Shader> planeShader;
    std::unique_ptr<Texture> texture;
    std::unique_ptr<Texture> texture2;
    std::unique_ptr<VertexBuffer> m_VB;
    std::unique_ptr<VertexBuffer> m_planeVB;


    glm::mat4 m_Project;
    glm::mat4 m_View;
    glm::mat4 m_Model;
    Camera camera;
};
}
