#include "tests/TestBlend.h"
#include "VertexBufferLayout.h"

#include <map>
#include <GLFw/glfw3.h>
namespace test
{
	TestBlend::TestBlend()
		:camera(glm::vec3(0.0f, 0.0f, 5.0f))
	{
	
		float cube[] = 
		{
			//λ��					//����		//������
			// Back face
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 0.0f, -1.0f,  // Bottom-left 0
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f, 0.0f, -1.0f,  // top-right 1
			 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 0.0f, -1.0f,  // bottom-right 2
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 0.0f, -1.0f,  // top-left 3
			// Front face
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,   // bottom-left 4
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f,   // bottom-right 5
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f,   // top-right 6
			-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f,   // top-left 7
			// Left face
			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f, 0.0f, 0.0f,   // top-right 8
			-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, -1.0f, 0.0f, 0.0f,   // top-left 9
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f, 0.0f, 0.0f,   // bottom-left 10
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, -1.0f, 0.0f, 0.0f,   // bottom-right 11
			// Right face
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f,   // top-left 12
			 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f,   // bottom-right 13
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f,   // top-right 14
			 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f,   // bottom-left 15
			// Bottom face
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f, 0.0f, 0.0f,  // top-right 16
			 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, -1.0f, 0.0f, 0.0f,  // top-left 17
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, -1.0f, 0.0f, 0.0f,  // bottom-left 18
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, -1.0f, 0.0f, 0.0f,  // bottom-right 19
			// Top face
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f,   // top-left 20
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f,   // bottom-right 21
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f,   // top-right 22
			-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f,   // bottom-left 23
		};
		
		unsigned int indices[] = 
		{
			0, 1, 2, // Back face
			1, 0, 3,
			4, 5, 6, // Front face
			6, 7, 4,
			8, 9, 10, // Left face
			10, 11, 8,
			12, 13, 14, // Right face
			13, 12, 15,
			16, 17, 18, // Bottom face
			18, 19, 16,
			20, 21, 22, // Top face
			21, 20, 23
		};

		float window[] =
		{
			//λ��				//����	   //������
			0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
			0.0f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f,
			1.0f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
			1.0f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f
		};
		unsigned int windowIndices[] =
		{
			0, 1, 2, 2, 3, 0
		};

		this->cubeShader = std::make_unique<Shader>("res/shaders/TestBlend/blend.shader");
		
		this->cubeVao = std::make_unique<VertexArray>();
		this->cubeIbo = std::make_unique<IndexBuffer>(indices, 6 * 6);
		this->cubeVb = std::make_unique<VertexBuffer>(cube, sizeof(cube));
		this->cubeTexture = std::make_unique<Texture>("res/textures/wall.jpg");
		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(2);
		layout.Push<float>(3);
		this->cubeVao->AddBuffer(*this->cubeVb, layout);
		this->cubeVao->UnBind();

		this->windowShader = std::make_unique<Shader>("res/shaders/TestBlend/blend.shader");
		this->windowVao = std::make_unique<VertexArray>();
		this->windowIbo = std::make_unique<IndexBuffer>(windowIndices, 6);
		this->windowVb = std::make_unique<VertexBuffer>(window, sizeof(window));
		this->windowTexture = std::make_unique<Texture>("res/textures/blending_transparent_window.png");
		VertexBufferLayout winLayout;
		winLayout.Push<float>(3);
		winLayout.Push<float>(2);
		winLayout.Push<float>(3);
		this->windowVao->AddBuffer(*this->windowVb, winLayout);
		this->windowVao->UnBind();
		
		
		this->cubeShader->Bind();
		this->cubeShader->SetUniform1i("tex", 0);


		//this->windowShader->Bind();
		//this->windowShader->SetUniform1i("tex", 0);
	}

	void TestBlend::OnUpdate(float deltatime)
	{

	}

	void TestBlend::OnRender()
	{

		GLCALL(glEnable(GL_DEPTH_TEST));
		glEnable(GL_BLEND);
		GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CCW);
		GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		Render render;
		
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)960.0 / (float)720.0, 0.1f, 100.0f);
		
		//this->cubeVao->Bind();
		this->cubeShader->Bind();
		this->cubeTexture->Bind();
		this->cubeShader->SetUniformMat4f("model", model);
		this->cubeShader->SetUniformMat4f("view", view);
		this->cubeShader->SetUniformMat4f("projection", projection);

		render.Draw(*this->cubeVao, *this->cubeIbo, *this->cubeShader);

		//window
		std::vector<glm::vec3> windows
		{
			glm::vec3(-1.5f, 0.0f, -0.48f),
			glm::vec3(1.5f, 0.0f, 0.51f),
			glm::vec3(0.0f, 0.0f, 0.7f),
			glm::vec3(-0.3f, 0.0f, -2.3f),
			glm::vec3(0.5f, 0.0f, -0.6f)
		};
		// sort the transparent windows before rendering
		//map���Զ����ݼ�ֵ(Key)������ֵ����(����)
		std::map<float, glm::vec3> sorted;
		for (unsigned int i = 0; i < windows.size(); i++)
		{
			float distance = glm::length(camera.Position - windows[i]);
			sorted[distance] = windows[i];
		}

		this->cubeShader->Bind();
		this->windowVao->Bind();
		this->windowTexture->Bind();
		for (std::map<float, glm::vec3>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, it->second);
			this->cubeShader->SetUniformMat4f("model", model);
			render.Draw(*this->windowVao, *this->windowIbo, *this->cubeShader);
		}

	}

	void TestBlend::OnImGuiRender()
	{

	}

	std::string TestBlend::GetTestName()
	{
		return "TestBlend";
	}

	void TestBlend::SetCamera(Camera& camera)
	{
		this->camera = camera;
	}

	TestBlend::~TestBlend()
	{
		this->cubeShader->UnBind();
		this->cubeVao->UnBind();
		this->cubeIbo->UnBind();
		this->cubeVb->UnBind();
		this->cubeTexture->UnBind();

		this->windowShader->UnBind();
		this->windowVao->UnBind();
		this->windowIbo->UnBind();
		this->windowVb->UnBind();
		this->windowTexture->UnBind();
	}

}