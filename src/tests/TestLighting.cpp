#include "tests/TestLighting.h"
#include "VertexBufferLayout.h"
#include "GLFW/glfw3.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <iostream>
#include <memory>
///心得
///当程序中报Warning: uniform viewPos doesn't exist 
/// Warning: uniform material.shininess doesn't exist
/// 类似的错误，这不是没有找到这个变量，而是找到了，但是后面的代码没有用到变量的值，以至于系统将它们
/// 优化


#define TYPE 0

namespace test {
	TestLighting::TestLighting():
		//cameraPos(0.0f, 0.0f, 5.0f),
		//cameraFront(0.0f, 0.0f, -1.0f),
		//cameraUp(0.0f, 1.0f, 0.0f),
		lightPos(1.2f, 1.0f, 2.0f),
		lightColor(1.0f, 1.0f, 1.0f),
		camera(glm::vec3(0.0f, 0.0f, 3.0f))
	{
		float cube[] = {
			//位置					//纹理		//法向量
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 0.0f, -1.0f,

			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f,

			-0.5f, -0.5f, -0.5f,  1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

			 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 
			 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 

			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 
			-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f
		};

		float vertices[] = {
			-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
			0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
			0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
			0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
			-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f,
			-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,

			-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
			0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
			0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,

			-0.5f, 0.5f, 0.5f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
			-0.5f, 0.5f, -0.5f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
			-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
			-0.5f, 0.5f, 0.5f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,

			0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
			0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
			0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
			0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
			0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
			0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,

			-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
			0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f,
			0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
			0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
			-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
			-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,

			-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
			0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
			0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
			0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
			-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
			-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f
		};

		unsigned int indices[] = {
			0, 1, 2, 2, 3, 0,

			4, 5, 6, 6, 7, 4,

			8, 9, 10, 10, 11, 8,

			12, 13, 14, 14, 15, 12,

			16, 17, 18, 18, 19, 16,

			20, 21, 22, 22, 23, 20
		};

		GLCALL(glEnable(GL_BLEND));
		GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
#if TYPE
		this->m_VB = std::make_unique<VertexBuffer>(vertices, 6 * 6 * 8 * sizeof(float));
#else
		this->m_VB = std::make_unique<VertexBuffer>(cube, sizeof(cube));
		this->m_IB = std::make_unique<IndexBuffer>(indices, 6 * 6);
#endif
		this->m_VAO = std::make_unique<VertexArray>();
		this->m_LightVAO = std::make_unique<VertexArray>();
		//获取着色器
		this->shader = std::make_unique<Shader>("res/shaders/Material.shader");
		this->lightShader = std::make_unique<Shader>("res/shaders/Light.shader");
		//顶点数组
		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(2);
		layout.Push<float>(3);

		this->m_VAO->AddBuffer(*this->m_VB, layout);

		this->m_LightVAO->AddBuffer(*this->m_VB, layout);

		this->texture = std::make_unique<Texture>("res/textures/container2.png");
		this->texture2 = std::make_unique<Texture>("res/textures/container2_specular.png");
		this->texture3 = std::make_unique<Texture>("res/textures/matrix.jpg");

		this->shader->Bind();
		this->shader->SetUniform1i("material.diffuse", 0);
		this->shader->SetUniform1i("material.specular", 1);
		this->shader->SetUniform1i("material.emission", 2);
	}

	TestLighting::~TestLighting()
	{
		this->texture->UnBind();
		this->texture2->UnBind();
		this->texture3->UnBind();
		this->m_VAO->UnBind();
		this->m_VB->UnBind();
		this->m_IB->UnBind();
		this->m_LightVAO->UnBind();

		this->shader->UnBind();
		this->lightShader->UnBind();
	}

	void TestLighting::OnUpdate(float deltatime)
	{

	}

	void TestLighting::OnRender()
	{
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		Render render;
		this->shader->Bind();
		this->shader->SetUniform3f("light.position", lightPos);
		this->shader->SetUniform3f("viewPos", this->camera.Position);
		//light properties
		this->shader->SetUniform3f("light.ambient", 0.2f, 0.2f, 0.2f);
		this->shader->SetUniform3f("light.diffuse", 0.5f, 0.5f, 0.2f);
		this->shader->SetUniform3f("light.specular", 1.0f, 1.0f, 1.0f);
		
		this->shader->SetUniform3f("lightColor", this->lightColor);
		//material properties
		this->shader->SetUniform1f("material.shininess", 64.0f);
		//纹理流动
		this->shader->SetUniform1f("time", static_cast<float>(glfwGetTime()));
		//std::cout << glfwGetTime() << std::endl;


		//glm::mat4 projection = glm::perspective(glm::radians(55.0f), (float)800 / (float)600, 0.1f, 100.0f);
		//glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

		glm::mat4 projection = glm::perspective(glm::radians(this->camera.Zoom), (float)800 / (float)600, 0.1f, 100.0f);
		glm::mat4 view = glm::lookAt(this->camera.Position, this->camera.Position + this->camera.Front, this->camera.Up);

		this->shader->SetUniformMat4f("projection", projection);
		this->shader->SetUniformMat4f("view", view);
		glm::mat4 model = glm::rotate(glm::mat4(1.0f), (float)glfwGetTime() * glm::radians(45.0f), glm::vec3(0.5f, 1.0f, 0.0f));
		this->shader->SetUniformMat4f("model", model);

		//漫反射贴图
		this->texture->Bind(0);
		//镜面反射贴图
		this->texture2->Bind(1);
		//放射光贴图
		this->texture3->Bind(2);

		this->m_VAO->Bind();
#if TYPE
		render.Draw(*this->m_VAO, *this->shader, 0, 36);
#else
		render.Draw(*this->m_VAO, *this->m_IB, *this->shader);
#endif
		//灯
		this->lightShader->Bind();
		model = glm::translate(glm::mat4(1.0f), lightPos);
		model = glm::scale(model, glm::vec3(0.2f));
		this->lightShader->SetUniformMat4f("model", model);
		this->lightShader->SetUniformMat4f("view", view);
		this->lightShader->SetUniformMat4f("projection", projection);
		this->m_LightVAO->Bind();
#if TYPE
		//render.Draw(*this->m_LightVAO, *this->lightShader, 0, 36);
#else
		render.Draw(*this->m_LightVAO, *this->m_IB, *this->lightShader);
#endif
	}

	void TestLighting::OnImGuiRender()
	{
		ImGui::Begin("OpenGL Texture Test");
		//ImGui::SliderFloat3("cameraPos", &this->cameraPos.x, -50.0f, 100.0f);
		//ImGui::SliderFloat3("cameraFront", &this->cameraFront.x, -50.0f, 50.0f);
		//ImGui::SliderFloat3("cameraUp", &this->cameraUp.x, -100.0f, 100.0f);
		ImGui::SliderFloat3("cameraPos", &this->camera.Position.x, -100.0f, 100.f);
		ImGui::SliderFloat3("lightPos", &this->lightPos.x, -100.0f, 100.0f);
		ImGui::ColorEdit3("lightColor", &this->lightColor.x);
		ImGui::End();
	}
	Camera TestLighting::GetCamera()
	{
		return this->camera;
	}
	void TestLighting::SetCamera(Camera& camera)
	{
		this->camera = camera;
	}
};
