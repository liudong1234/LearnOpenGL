#include "tests/TestStencil.h"
#include "GLFW/glfw3.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "glm/gtc/matrix_transform.hpp"
#include <iostream>
#include <memory>

namespace test
{
	TestStencil::TestStencil() :
		m_Project(glm::mat4(1.0f)),
		m_View(glm::mat4(1.0f)),
		m_Model(glm::mat4(1.0f)),
		camera(glm::vec3(0.0f, 0.0f, 5.0f)),
		angle(180.0f)
	{

		GLCALL(glEnable(GL_BLEND));
		GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		GLCALL(glEnable(GL_DEPTH_TEST));
		float cube[] =
		{
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

		float planeVertices[] =
		{
			// positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
			5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
			-5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
			-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

			5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
			-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
			5.0f, -0.5f, -5.0f,  2.0f, 2.0f
		};

		float grassVertices[] =
		{
			// positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
			0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
			0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
			1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

			0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
			1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
			1.0f,  0.5f,  0.0f,  1.0f,  0.0f
		};

		unsigned int indices[] = {
			0, 1, 2, 2, 3, 0,

			4, 5, 6, 6, 7, 4,

			8, 9, 10, 10, 11, 8,

			12, 13, 14, 14, 15, 12,

			16, 17, 18, 18, 19, 16,

			20, 21, 22, 22, 23, 20
		};

		//cube
		this->m_VAO = std::make_unique<VertexArray>();
		this->m_IB = std::make_unique<IndexBuffer>(indices, 6 * 6);
		//获取着色器
		this->shader = std::make_unique<Shader>("res/shaders/stencilCube.shader");
		//this->edgeShader = std::make_unique<Shader>("res/shaders/")
		//顶点数组
		this->m_VB = std::make_unique<VertexBuffer>(cube, 8 * 4 * 6 * sizeof(float));
		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(2);
		layout.Push<float>(3);
		this->m_VAO->AddBuffer(*this->m_VB, layout);

		//地板
		this->planeVAO = std::make_unique<VertexArray>();
		this->planeShader = std::make_unique<Shader>("res/shaders/stencilPlane.shader");
		this->m_planeVB = std::make_unique<VertexBuffer>(planeVertices, sizeof(planeVertices));
		VertexBufferLayout layoutPlane;
		layoutPlane.Push<float>(3);
		layoutPlane.Push<float>(2);
		this->planeVAO->AddBuffer(*this->m_planeVB, layoutPlane);

		//grass
		this->grassVAO = std::make_unique<VertexArray>();
		this->grassShader = std::make_unique<Shader>("res/shaders/grass.shader");
		this->m_grassVB = std::make_unique<VertexBuffer>(grassVertices, sizeof(grassVertices));
		VertexBufferLayout layoutGrass;
		layoutGrass.Push<float>(3);
		layoutGrass.Push<float>(2);
		this->grassVAO->AddBuffer(*this->m_grassVB, layoutGrass);

		//纹理
		this->cubeTexture = std::make_unique<Texture>("res/textures/container2.png");
		this->planeTexture = std::make_unique<Texture>("res/textures/wall.jpg");
		this->grassTexture = std::make_unique<Texture>("res/textures/grass.png");
		this->edgeTexture = std::make_unique<Texture>("res/textures/container2_specular.png");
		
		this->shader->Bind();
		this->shader->SetUniform1i("tex", 0);
		this->planeShader->Bind();
		this->planeShader->SetUniform1i("tex", 0);
		this->grassShader->Bind();
		this->grassShader->SetUniform1i("tex", 0);
	}

	void TestStencil::OnUpdate(float deltatime)
	{

	}

	void TestStencil::RenderCube(Render& render)
	{
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);
		glDepthMask(GL_FALSE);  // 关闭深度写入

		glm::mat4 model = glm::mat4(1.0f);

		this->shader->Bind();
		this->m_VAO->Bind();
		this->cubeTexture->Bind();
		this->shader->SetUniformMat4f("projection", this->m_Project);
		this->shader->SetUniformMat4f("view", this->m_View);
		// world transformation
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
		this->shader->SetUniformMat4f("model", model);
		render.Draw(*this->m_VAO, *this->m_IB, *this->shader);
		glDepthMask(GL_TRUE);  // 深度写入
	}

	void TestStencil::RenderCubeEdge(Render& render)
	{
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);//在它们片段的模板值不等于1时才绘制。
		glStencilMask(0x00);
		glDepthMask(GL_FALSE);  // 关闭深度写入
		this->shader->Bind();
		this->edgeTexture->Bind();
		float scale = 1.1f;
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
		model = glm::scale(model, glm::vec3(scale, scale, scale));

		this->shader->SetUniformMat4f("model", model);
		render.Draw(*this->m_VAO, *this->m_IB, *this->shader);
		glDepthMask(GL_TRUE);  // 深度写入
	}
	//地板
	void TestStencil::RenderPlane(Render& render)
	{
		glm::mat4 model = glm::mat4(1.0f);
		this->m_View = this->camera.GetViewMatrix();
		this->m_Project = glm::perspective(glm::radians(camera.Zoom), (float)960.0 / (float)720.0, 0.1f, 100.0f);

		glStencilMask(0x00);
		this->planeShader->Bind();
		this->planeVAO->Bind();

		this->planeTexture->Bind();
		this->planeShader->SetUniformMat4f("view", this->m_View);
		this->planeShader->SetUniformMat4f("projection", this->m_Project);
		this->planeShader->SetUniformMat4f("model", model);
		render.Draw(*this->planeVAO, *this->planeShader, 0, 6);
	}

	void TestStencil::RenderGrass(Render& render)
	{
		glStencilMask(0x00);
		std::vector<glm::vec3> vegetation;
		vegetation.push_back(glm::vec3(-1.5f, 0.0f, -0.48f));
		vegetation.push_back(glm::vec3(1.5f, 0.0f, 0.51f));
		vegetation.push_back(glm::vec3(0.0f, 0.0f, 0.7f));
		vegetation.push_back(glm::vec3(-0.3f, 0.0f, -2.3f));
		vegetation.push_back(glm::vec3(0.5f, 0.0f, -0.6f));

		glm::mat4 model = glm::mat4(1.0f);
		this->grassShader->Bind();
		this->grassVAO->Bind();
		this->grassTexture->Bind();
		this->grassShader->SetUniformMat4f("projection", this->m_Project);
		this->grassShader->SetUniformMat4f("view", this->m_View);

		for (unsigned int i = 0; i < vegetation.size(); i++)
		{
			model = glm::mat4(1.0f);
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::translate(model, vegetation[i]);
			this->grassShader->SetUniformMat4f("model", model);
			GLCALL(render.Draw(*this->grassVAO, *this->grassShader, 0, 6));
		}
	}

	void TestStencil::OnRender()
	{
		glEnable(GL_STENCIL_TEST);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		GLCALL(glEnable(GL_BLEND));
		GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		GLCALL(glEnable(GL_DEPTH_TEST));

		glClearColor(0.5f, 0.75f, 0.75f, 1.0f);
		GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));

		Render render;
		//先渲染地板
		this->RenderPlane(render);
		// 第一组立方体
		this->RenderCube(render);
		//渲染草
		this->RenderGrass(render);

		//第二组立方体轮廓
		this->RenderCubeEdge(render);
		glStencilMask(0xFF);
		//glEnable(GL_DEPTH_TEST);
		glDisable(GL_STENCIL_TEST);
	}

	void TestStencil::OnImGuiRender()
	{
		if (ImGui::CollapsingHeader("positionSettings"))
		{
			ImGui::SliderFloat3("cameraPos", &this->camera.Position.x, -100, 100);
			ImGui::SliderFloat3("cameraFront", &this->camera.Front.x, -100, 100);
			ImGui::SliderFloat3("cameraUp", &this->camera.Up.x, -10, 10);
		}
		
		ImGui::SliderFloat("angle", &this->angle, 0, 360);


		ImGui::Text("application %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);               // Display some text (you can use a format strings too)

	}

	std::string TestStencil::GetTestName()
	{
		return "TestStencil";
	}

	void TestStencil::SetCamera(Camera& camera)
	{
		this->camera = camera;
	}

	TestStencil::~TestStencil()
	{
		this->shader->UnBind();
		this->planeShader->UnBind();
		this->grassShader->UnBind();

		this->m_IB->UnBind();
		this->m_VAO->UnBind();
		this->planeVAO->UnBind();
		this->grassVAO->UnBind();

		this->m_VB->UnBind();
		this->m_planeVB->UnBind();
		this->m_grassVB->UnBind();

		this->cubeTexture->UnBind();
		this->planeTexture->UnBind();
		this->grassTexture->UnBind();
		this->edgeTexture->UnBind();

	}
}