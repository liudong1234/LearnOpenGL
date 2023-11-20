#include "tests/TestFrameBuffer.h"
#include "VertexBufferLayout.h"

#include <iostream>

namespace test
{

	TestFrameBuffer::TestFrameBuffer():
		camera(glm::vec3(0.0f, 0.0f, 5.0f))
	{
		float cube[] =
		{
			//λ��					//����		//������
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

		unsigned int indices[] = 
		{
			0, 1, 2, 2, 3, 0,

			4, 5, 6, 6, 7, 4,

			8, 9, 10, 10, 11, 8,

			12, 13, 14, 14, 15, 12,

			16, 17, 18, 18, 19, 16,

			20, 21, 22, 22, 23, 20
		};

		float quadVertices[] = 
		{ // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
			// positions   // texCoords
			-1.0f,  1.0f,  0.0f, 1.0f,
			-1.0f, -1.0f,  0.0f, 0.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,

			-1.0f,  1.0f,  0.0f, 1.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,
			 1.0f,  1.0f,  1.0f, 1.0f
		};


		this->shader = std::make_unique<Shader>("res/shaders/TestFrameBuffer/basic.shader");
		this->screenShader = std::make_unique<Shader>("res/shaders/TestFrameBuffer/framebuffer.shader");
		//cube
		this->cubeVao = std::make_unique<VertexArray>();
		this->cubeIbo = std::make_unique<IndexBuffer>(indices, 6 * 6);
		this->cubeVb = std::make_unique<VertexBuffer>(cube, sizeof(cube));
		this->cubeTexuture = std::make_unique<Texture>("res/textures/container.jpg");
		VertexBufferLayout cubeLayout;
		cubeLayout.Push<float>(3);
		cubeLayout.Push<float>(2);
		cubeLayout.Push<float>(3);
		this->cubeVao->AddBuffer(*this->cubeVb, cubeLayout);
		
		//plane
		this->planeVao = std::make_unique<VertexArray>();
		this->planeVb = std::make_unique<VertexBuffer>(planeVertices, sizeof(planeVertices));
		this->planeTexuture = std::make_unique<Texture>("res/textures/wall.jpg");
		VertexBufferLayout planeLayout;
		planeLayout.Push<float>(3);
		planeLayout.Push<float>(2);
		this->planeVao->AddBuffer(*this->planeVb, planeLayout);


		this->shader->Bind();
		this->shader->SetUniform1i("tex", 0);
		this->screenShader->Bind();
		this->screenShader->SetUniform1i("screenTexture", 0);

		this->quadVao = std::make_unique<VertexArray>();
		this->quadVb = std::make_unique<VertexBuffer>(quadVertices, sizeof(quadVertices));
		VertexBufferLayout quadLayout;
		quadLayout.Push<float>(2);
		quadLayout.Push<float>(2);
		this->quadVao->AddBuffer(*this->quadVb, quadLayout);

		//֡����
		this->frameBuffer = std::make_unique<FrameBuffer>();
		this->frameBuffer->Bind();
		//������������
		this->quadTexture = std::make_unique<Texture>();
		this->quadTexture->Bind();

		//����������
		this->frameBuffer->BindTexture2D(this->quadTexture->GetID());
		//����Ϊһ����Ⱥ�ģ�帽����Ⱦ�������

		unsigned int rbo;
		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//this->cubeVao->UnBind();
		//this->planeVao->UnBind();
		//this->quadVao->UnBind();
		//this->skysboxVao->UnBind();
	}

	TestFrameBuffer::~TestFrameBuffer()
	{
		this->shader->UnBind();
		this->screenShader->UnBind();

		this->cubeVao->UnBind();
		this->planeVao->UnBind();
		this->quadVao->UnBind();

		this->cubeVb->UnBind();
		this->planeVb->UnBind();
		this->quadVb->UnBind();

		this->cubeTexuture->UnBind();
		this->planeTexuture->UnBind();
		this->quadTexture->UnBind();

		this->cubeIbo->UnBind();
		this->frameBuffer->UnBind();

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//�������ģʽ
	}

	void TestFrameBuffer::OnUpdate(float deltatime)
	{

	}

	void TestFrameBuffer::OnRender()
	{
		this->frameBuffer->Bind();
		glEnable(GL_DEPTH_TEST);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//�߿�ģʽ
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)800 / (float)600, 0.1f, 100.0f);
		Render render;

		this->shader->Bind();
		this->shader->SetUniformMat4f("view", view);
		this->shader->SetUniformMat4f("projection", projection);

		//cube
		this->cubeVao->Bind();
		this->cubeTexuture->Bind();
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
		this->shader->SetUniformMat4f("model", model);
		render.Draw(*this->cubeVao, *this->cubeIbo, *this->shader);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
		this->shader->SetUniformMat4f("model", model);
		render.Draw(*this->cubeVao, *this->cubeIbo, *this->shader);

		//floor
		this->planeVao->Bind();
		this->planeTexuture->Bind();
		model = glm::mat4(1.0f);
		shader->SetUniformMat4f("model", model);
		render.Draw(*this->planeVao, *this->shader, 0, 6);


		// now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
		this->frameBuffer->UnBind();
		glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
		// clear all relevant buffers
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessary actually, since we won't be able to see behind the quad anyways)
		glClear(GL_COLOR_BUFFER_BIT);

		this->screenShader->Bind();
		this->quadVao->Bind();
		this->quadTexture->Bind();	// use the color attachment texture as the texture of the quad plane
		glDrawArrays(GL_TRIANGLES, 0, 6);

	}

	void TestFrameBuffer::OnImGuiRender()
	{  
	}

	std::string TestFrameBuffer::GetTestName()
	{
		return "TestFrameBuffer";
	}

	void TestFrameBuffer::SetCamera(Camera& camera)
	{
		this->camera = camera;
	}

}