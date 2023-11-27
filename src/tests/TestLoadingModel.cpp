#include "tests/TestLoadingModel.h"
#include "VertexBufferLayout.h"
#include "tests/Log.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <glfw/glfw3.h>
#include <iostream>
namespace test
{
	TestLoadingModel::TestLoadingModel():
		modelEx("res/model/Nahida/scene.gltf"),
		camera(glm::vec3(0.0f, 0.0f, 3.0f)),
		lightPos(1.2f, 1.0f, 2.0f)
	{
		
		//��պ�������ַ
		std::vector<std::string> faces
		{
			"res/Textures/skybox/right.png",
			"res/Textures/skybox/left.png",
			"res/Textures/skybox/top.png",
			"res/Textures/skybox/bottom.png",
			"res/Textures/skybox/front.png",
			"res/Textures/skybox/back.png"
		};

		float skyboxVertices[] =
		{
			// positions          
			-1.0f,  1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			-1.0f,  1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			 1.0f,  -1.0f,  1.0f
		};

		//��պ�
		this->skysboxShader = std::make_unique<Shader>("res/shaders/TestLoadingModel/Skybox.shader");
		this->skysboxTexture = std::make_unique<Texture>(faces);
		this->skysboxVao = std::make_unique<VertexArray>();
		this->skysboxVb = std::make_unique<VertexBuffer>(skyboxVertices, sizeof(skyboxVertices));
		VertexBufferLayout skysboxLayout;
		skysboxLayout.Push<float>(3);
		this->skysboxVao->AddBuffer(*this->skysboxVb, skysboxLayout);

		//other
		this->shader = std::make_unique<Shader>("res/shaders/TestLoadingModel/Basic.shader");
		this->m_VAO = std::make_unique<VertexArray>();

		this->skysboxShader->Bind();
		this->skysboxShader->SetUniform1i("skybox", 0);

		this->shader->Bind();
		this->shader->SetUniform1i("skybox", 0);
	}

	void TestLoadingModel::OnUpdate(float deltatime)
	{

	}

	void TestLoadingModel::OnRender()
	{
		GLCALL(glEnable(GL_DEPTH_TEST));

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		glm::mat4 projection = glm::perspective(glm::radians(this->camera.Zoom), (float)800 / (float)600, 0.1f, 100.0f);
		glm::mat4 view = this->camera.GetViewMatrix();
		glm::mat4 model = glm::mat4(1.0f);

		Render render;
		this->shader->Bind();
		this->m_VAO->Bind();
		this->shader->SetUniformMat4f("projection", projection);
		this->shader->SetUniformMat4f("view", view);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		model = glm::rotate(model, (float)glfwGetTime() * glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		this->shader->SetUniformMat4f("model", model);

		this->shader->SetUniform3f("cameraPos", this->camera.Position);
		this->shader->SetUniform3f("light.position", this->lightPos);
		//light properties
		this->shader->SetUniform3f("light.ambient", 0.6f, 0.6f, 0.6f);
		this->shader->SetUniform3f("light.diffuse", 0.8f, 0.7f, 0.6f);
		this->shader->SetUniform3f("light.specular", 0.1f, 0.1f, 0.1f);
		this->shader->SetUniform1f("material.shininess", 32.0f);

		GLCALL(modelEx.Draw(*this->shader));
		this->m_VAO->UnBind();


		glDepthFunc(GL_LEQUAL);
		this->skysboxShader->Bind();
		view = glm::mat4(glm::mat3(this->camera.GetViewMatrix()));
		this->skysboxShader->SetUniformMat4f("view", view);
		this->skysboxShader->SetUniformMat4f("projection", projection);
		this->skysboxTexture->CubeBind();

		render.Draw(*this->skysboxVao, *this->skysboxShader, 0, 36);
		this->skysboxVao->UnBind();
		glDepthFunc(GL_LESS);

	}

	void TestLoadingModel::OnImGuiRender()
	{
		ImGui::DragFloat3(u8"灯光", &this->lightPos.x, 1.0f, -100.0f, 100.0f);
	}

	void TestLoadingModel::SetCamera(Camera& camera)
	{
		this->camera = camera;
	}

	TestLoadingModel::~TestLoadingModel()
	{
		this->shader->UnBind();
		this->m_VAO->UnBind();
		this->m_VB->UnBind();
		this->m_IB->UnBind();

		this->skysboxShader->UnBind();
		this->skysboxTexture->UnBind();
		this->skysboxVao->UnBind();

		LOG(this->GetTestName() + "析构")
	}

}