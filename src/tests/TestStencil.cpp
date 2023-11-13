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
		camera(glm::vec3(0.0f, 0.0f, 5.0f))
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

        float planeVertices[] = {
        // positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
         5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
        -5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

         5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
         5.0f, -0.5f, -5.0f,  2.0f, 2.0f
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
		glEnable(GL_DEPTH_TEST);

		this->m_VAO = std::make_unique<VertexArray>();
		this->m_IB = std::make_unique<IndexBuffer>(indices, 6 * 6);
		//获取着色器
		this->shader = std::make_unique<Shader>("res/shaders/stencilCube.shader");
		//顶点数组
		this->m_VB = std::make_unique<VertexBuffer>(cube,8 * 4 * 6 * sizeof(float));
		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(2);
		layout.Push<float>(3);
		this->m_VAO->AddBuffer(*this->m_VB, layout);
		
		this->planeVAO = std::make_unique<VertexArray>();
        this->planeShader = std::make_unique<Shader>("res/shaders/stencilPlane.shader");
        this->m_planeVB = std::make_unique<VertexBuffer>(planeVertices, sizeof(planeVertices));
        VertexBufferLayout layoutPlane;
        layoutPlane.Push<float>(3);
        layoutPlane.Push<float>(2);
        this->planeVAO->AddBuffer(*this->m_planeVB, layoutPlane);


		//纹理
		this->texture = std::make_unique<Texture>("res/textures/container2.png");
		this->texture2 = std::make_unique<Texture>("res/textures/container2_specular.png");
		
		this->shader->Bind();
        this->shader->SetUniform1i("texture1", 0);
        this->shader->SetUniform1i("texture2", 0);
	}

	void TestStencil::OnUpdate(float deltatime)
	{
		this->shader->UnBind();
		this->m_VAO->UnBind();
		this->m_VB->UnBind();
		this->texture->UnBind();
		this->texture2->UnBind();
	}

	void TestStencil::OnRender()
	{
		glClearColor(0.5f, 0.75f, 0.75f, 1.0f);
		
		glEnable(GL_STENCIL_TEST);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

		
		GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT ));

		Render render;
		
        //先渲染地板
        this->planeShader->Bind();
        glm::mat4 model = glm::mat4(1.0f);
		this->m_View = this->camera.GetViewMatrix();
		this->m_Project = glm::perspective(glm::radians(camera.Zoom), (float)960.0 / (float)720.0, 0.1f, 100.0f);
        this->planeShader->SetUniformMat4f("view", this->m_View);
        this->planeShader->SetUniformMat4f("projection", this->m_Project);
        
        glStencilMask(0x00);
        this->planeVAO->Bind();
        this->texture2->Bind();
        this->planeShader->SetUniformMat4f("model", model);
        render.Draw(*this->planeVAO, *this->planeShader,0, 6);
        this->planeVAO->UnBind();

    // 第一组立方体
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilMask(0xFF);
    
		this->shader->Bind();
		this->m_VAO->Bind();
        this->texture->Bind();
		this->shader->SetUniformMat4f("projection", this->m_Project);
		this->shader->SetUniformMat4f("view", this->m_View);
		// world transformation
		model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
		this->shader->SetUniformMat4f("model", model);
        render.Draw(*this->m_VAO, *this->m_IB, *this->shader);
        
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
        this->shader->SetUniformMat4f("model", model);
        render.Draw(*this->m_VAO, *this->m_IB, *this->shader);
    //第二组立方体轮廓
        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);//在它们片段的模板值不等于1时才绘制。
        glStencilMask(0x00);
        glDisable(GL_DEPTH_TEST);

        this->planeShader->Bind();
		float scale = 1.1f;
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
        model = glm::scale(model, glm::vec3(scale, scale, scale));

        this->planeShader->SetUniformMat4f("model", model);
        render.Draw(*this->m_VAO, *this->m_IB, *this->planeShader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(scale, scale, scale));

        this->shader->SetUniformMat4f("model", model);
        render.Draw(*this->m_VAO, *this->m_IB, *this->planeShader);
        this->m_VAO->UnBind();

        glStencilMask(0xFF);
        glStencilFunc(GL_ALWAYS, 0, 0xFF);
        // glEnable(GL_DEPTH_TEST);

	}

	void TestStencil::OnImGuiRender()
	{
		if (ImGui::CollapsingHeader("positionSettings"))
		{
			ImGui::SliderFloat3("cameraPos", &this->camera.Position.x, -100, 100);
			ImGui::SliderFloat3("cameraFront", &this->camera.Front.x, -100, 100);
			ImGui::SliderFloat3("cameraUp", &this->camera.Up.x, -10, 10);
		}


		ImGui::Text("application %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);               // Display some text (you can use a format strings too)

	}
	
	std::string TestStencil::GetTestName()
	{
		return "TestTexture2D";
	}
	
	void TestStencil::SetCamera(Camera& camera)
	{
		this->camera = camera;
	}
	
	TestStencil::~TestStencil()
	{
		this->texture->UnBind();
		this->texture2->UnBind();
		this->m_VAO->UnBind();
		this->m_VB->UnBind();
		this->m_IB->UnBind();

		this->shader->UnBind();
	}
}