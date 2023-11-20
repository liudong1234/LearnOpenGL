#include "tests/TestTexture2D.h"
#include "GLFW/glfw3.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "glm/gtc/matrix_transform.hpp"
#include "tests/Log.h"

#include <iostream>
namespace test
{
	TestTexture2D::TestTexture2D() :
		m_Project(glm::mat4(1.0f)),
		m_View(glm::mat4(1.0f)),
		m_Model(glm::mat4(1.0f)),
		lightDirect(-0.2f, -1.0f, -0.3f),
		lightPos(1.2f, 1.0f, 2.0f),
		camera(glm::vec3(0.0f, 0.0f, 5.0f)),
		backgroundColor(0.0f, 0.0f, 0.0f, 1.0f),
		spotlightAmbient(0.2f, 0.2f, 0.2f),
		spotlightDiffuse(0.5f, 0.5f, 0.2f),
		spotlightSpecular(1.0f, 1.0f, 1.0f),
		materialDiffuse(0),
		materialSpecular(1)
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

		unsigned int indices[] = {
			0, 1, 2, 2, 3, 0,
			
			4, 5, 6, 6, 7, 4,

			8, 9, 10, 10, 11, 8,

			12, 13, 14, 14, 15, 12,

			16, 17, 18, 18, 19, 16,

			20, 21, 22, 22, 23, 20
		};

		//获取着色器
		this->shader = std::make_unique<Shader>("res/shaders/TestTexture2D/LightType.shader");
		this->lightShader = std::make_unique<Shader>("res/shaders/TestTexture2D/light.shader");

		this->m_VAO = std::make_unique<VertexArray>();
		this->lightVAO = std::make_unique<VertexArray>();
		this->m_IB = std::make_unique<IndexBuffer>(indices, 6 * 6);

		//顶点数组
		this->m_VB = std::make_unique<VertexBuffer>(cube,8 * 4 * 6 * sizeof(float));
		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(2);
		layout.Push<float>(3);
		this->m_VAO->AddBuffer(*this->m_VB, layout);
		this->lightVAO->AddBuffer(*this->m_VB, layout);

		//纹理
		this->texture = std::make_unique<Texture>("res/textures/container2.png");
		this->texture2 = std::make_unique<Texture>("res/textures/container2_specular.png");
		this->texture3 = std::make_unique<Texture>("res/textures/head.png");
		
		this->m_VAO->UnBind();
		this->lightVAO->UnBind();
		
	}

	TestTexture2D::~TestTexture2D()
	{
		this->shader->UnBind();
		this->lightShader->UnBind();
		this->texture->UnBind();
		this->texture2->UnBind();
		this->texture3->UnBind();
		this->m_VAO->UnBind();
		this->m_VB->UnBind();
		this->m_IB->UnBind();

		LOG(this->GetTestName() + " 析构")
	}

	void TestTexture2D::OnUpdate(float deltatime)
	{
	}

	void TestTexture2D::OnRender()
	{
		GLCALL(glEnable(GL_BLEND));
		GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		GLCALL(glEnable(GL_DEPTH_TEST));

		glClearColor(this->backgroundColor.x, this->backgroundColor.y, this->backgroundColor.z, this->backgroundColor.w);
		GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));

		Render render;
		glm::vec3 cubePositions[] = {
			glm::vec3(2.0f,  0.0f,  -18.0f),
			glm::vec3(2.0f,  5.0f, -15.0f),
			glm::vec3(-1.5f, -2.2f, -2.5f),
			glm::vec3(-3.8f, -2.0f, -12.3f),
			glm::vec3(2.4f, -0.4f, -3.5f),
			glm::vec3(-1.7f,  3.0f, -7.5f),
			glm::vec3(1.3f, -2.0f, -2.5f),
			glm::vec3(1.5f,  2.0f, -2.5f),
			glm::vec3(1.5f,  0.2f, -1.5f),
			glm::vec3(-1.3f,  1.0f, -1.5f)
		};

		this->shader->Bind();
		this->shader->SetUniform1i("material.diffuse", this->materialDiffuse);
		this->shader->SetUniform1i("material.specular", this->materialSpecular);
		this->shader->SetUniform1i("picture", 2);

		this->shader->SetUniform3f("viewPos", this->camera.Position);
		//light properties
		//DirectionLight
		DirectionalLight directionLight;
		directionLight.direction = this->lightDirect;
		directionLight.ambient = glm::vec3(0.2f, 0.2f, 0.2f);
		directionLight.diffuse = glm::vec3(0.5f, 0.5f, 0.2f);
		directionLight.specular = glm::vec3(1.0f, 1.0f, 1.0f);
		this->shader->SetStructDirectionalLight(directionLight);
		//this->shader->SetUniform3f("parallelLight.direction", lightPos);
		//this->shader->SetUniform3f("parallelLight.ambient", 0.2f, 0.2f, 0.2f);
		//this->shader->SetUniform3f("parallelLight.diffuse", 0.5f, 0.5f, 0.2f);
		//this->shader->SetUniform3f("parallelLight.specular", 1.0f, 1.0f, 1.0f);
		
		//PointLight
		PointLight pointLight[4];
		glm::vec3 pointLightPositions[] = {
			glm::vec3(0.7f,  0.2f,  2.0f),
			glm::vec3(2.3f, -3.3f, -4.0f),
			glm::vec3(-4.0f,  2.0f, -12.0f),
			glm::vec3(0.0f,  0.0f, -3.0f)
		};

		for (int i = 0; i < sizeof(pointLightPositions) / sizeof(glm::vec3); i++)
		{
			pointLight[i].position = pointLightPositions[i];
			pointLight[i].ambient =  glm::vec3(0.2f, 0.2f, 0.2f);
			pointLight[i].diffuse =  glm::vec3(0.5f, 0.5f, 0.2f);
			pointLight[i].specular = glm::vec3(1.0f, 1.0f, 1.0f);
			pointLight[i].constant = 1.0f;
			pointLight[i].linear = 0.09f;
			pointLight[i].quadratic = 0.032f;
		}

		this->shader->SetStructPointLightArray(pointLight, sizeof(pointLightPositions) / sizeof(glm::vec3));
		//SpotLight
		SpotLight spotlight;
		spotlight.position = camera.Position;
		spotlight.direction = camera.Front;
		spotlight.ambient = this->spotlightAmbient;
		spotlight.diffuse = this->spotlightDiffuse;
		spotlight.specular = this->spotlightSpecular;
		spotlight.cutOff = glm::cos(glm::radians(12.5f));
		spotlight.outerCutOff = glm::cos(glm::radians(14.0f));
		this->shader->SetStructSpotLight(spotlight);

		this->shader->SetUniform1f("material.shininess", 64.0f);
		this->m_View = this->camera.GetViewMatrix();
		this->m_Project = glm::perspective(glm::radians(camera.Zoom), (float)960.0 / (float)720.0, 0.1f, 100.0f);
		
		this->shader->SetUniformMat4f("projection", this->m_Project);
		this->shader->SetUniformMat4f("view", this->m_View);
		// world transformation
		glm::mat4 model = glm::mat4(1.0f);
		this->shader->SetUniformMat4f("model", model);
		//this->shader->SetUniform1i("lightType", this->selectedItem);
		this->texture->Bind();
		this->texture2->Bind(1);
		this->texture3->Bind(2);
		this->m_VAO->Bind();
		for (unsigned int i = 0; i < 10; i++)
		{
			// calculate the model matrix for each object and pass it to shader before drawing
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			this->shader->SetUniformMat4f("model", model);
			render.Draw(*this->m_VAO, *this->m_IB, *this->shader);
		}
	}

	void TestTexture2D::OnImGuiRender()
	{
		if (ImGui::CollapsingHeader("colorSettings"))
		{
			ImGui::ColorEdit4(u8"背景颜色", &this->backgroundColor.x);

			if (ImGui::TreeNode(u8"手电筒"))
			{
				ImGui::ColorEdit4("ambient", &this->spotlightAmbient.x);
				ImGui::ColorEdit4("diffuse", &this->spotlightDiffuse.x);
				ImGui::ColorEdit4("specular", &this->spotlightSpecular.x);

				ImGui::TreePop();
			}
			if (ImGui::TreeNode(u8"材质"))
			{
				ImGui::DragFloat("diffuse", &this->materialDiffuse, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("specular", &this->materialSpecular, 0.01f, 0.0f, 1.0f);
				ImGui::TreePop();
			}
		}

		if (ImGui::CollapsingHeader("positionSettings"))
		{
			ImGui::SliderFloat3("lightDirection", &this->lightDirect.x, -100, 100);
			ImGui::SliderFloat3("cameraPos", &this->camera.Position.x, -100, 100);
			ImGui::SliderFloat3("cameraFront", &this->camera.Front.x, -100, 100);
			ImGui::SliderFloat3("cameraUp", &this->camera.Up.x, -10, 10);
		}


		ImGui::Text("application %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);               // Display some text (you can use a format strings too)

	}
	
	std::string TestTexture2D::GetTestName()
	{
		return "TestTexture2D";
	}
	
	void TestTexture2D::SetCamera(Camera& camera)
	{
		this->camera = camera;
	}
}