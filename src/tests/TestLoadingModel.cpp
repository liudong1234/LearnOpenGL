#include "tests/TestLoadingModel.h"
#include <glfw/glfw3.h>
#include "VertexBufferLayout.h"

namespace test
{
	TestLoadingModel::TestLoadingModel():
		modelEx("res/model/raiden/raiden.obj"),
		camera(glm::vec3(0.0f, 0.0f, 3.0f))
	{
		this->shader = std::make_unique<Shader>("res/shaders/Basic.shader");

		float planeVertices[] = {
			// positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
			 5.0f, 0.0f,  5.0f,  2.0f, 0.0f,
			-5.0f, 0.0f,  5.0f,  0.0f, 0.0f,
			-5.0f, 0.0f, -5.0f,  0.0f, 2.0f,
			 5.0f, 0.0f,  5.0f,  2.0f, 0.0f,
			-5.0f, 0.0f, -5.0f,  0.0f, 2.0f,
			 5.0f, 0.0f, -5.0f,  2.0f, 2.0f
		};
		this->planeShader = std::make_unique<Shader>("res/shaders/depth_testing.shader");
		this->m_VAO = std::make_unique<VertexArray>();
		
		this->m_VB = std::make_unique<VertexBuffer>(planeVertices, sizeof(planeVertices));

		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(2);

		this->m_VAO->AddBuffer(*this->m_VB, layout);

		this->planeTex = std::make_unique<Texture>("res/textures/wall.jpg");


	}

	void TestLoadingModel::OnUpdate(float deltatime)
	{

	}

	void TestLoadingModel::OnRender()
	{

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		// glEnable(GL_DEPTH_TEST);
		// glDepthMask(GL_FALSE);

		Render render;
		glm::mat4 projection = glm::perspective(glm::radians(this->camera.Zoom), (float)800 / (float)600, 0.1f, 100.0f);
		glm::mat4 view = this->camera.GetViewMatrix();
		glm::mat4 model0 = glm::mat4(1.0f);
		// model0 = glm::scale(model0, glm::vec3(10.5f, 10.5f, 10.5f));
		model0 = glm::translate(model0, glm::vec3(0.0f, -1.0f, 0.0f));
		this->planeShader->Bind();
		this->planeShader->SetUniformMat4f("projection", projection);
		this->planeShader->SetUniformMat4f("view", view);
		this->planeShader->SetUniformMat4f("model", model0);
		this->planeTex->Bind();
		this->m_VAO->Bind();

		this->shader->Bind();
		this->shader->SetUniformMat4f("projection", projection);
		this->shader->SetUniformMat4f("view", view);
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		model = glm::rotate(model, (float)glfwGetTime() * glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		this->shader->SetUniformMat4f("model", model);

		render.Draw(*this->m_VAO, *this->planeShader, 0, 36);
		GLCALL(modelEx.Draw(*this->shader));


	}

	void TestLoadingModel::OnImGuiRender()
	{

	}

	void TestLoadingModel::SetCamera(Camera& camera)
	{
		this->camera = camera;
	}

}