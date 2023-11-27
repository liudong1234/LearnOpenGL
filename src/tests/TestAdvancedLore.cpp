#include "tests/TestAdvancedLore.h"
#include "VertexBufferLayout.h"
#include "glm/gtc/type_ptr.hpp"

#include <sstream>

namespace test
{
	TestAdvancedLore::TestAdvancedLore() :
		camera(glm::vec3(0.0f, 0.0f, 3.0f))
	{
		this->cubeShader = std::make_unique<Shader>("res/shaders/TestAdvancedLore/cube.shader");
		this->cubeVao = std::make_unique<VertexArray>();

		float cube[] = {
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
		unsigned int indices[] = 
		{
			0, 1, 2, 2, 3, 0,

			4, 5, 6, 6, 7, 4,

			8, 9, 10, 10, 11, 8,

			12, 13, 14, 14, 15, 12,

			16, 17, 18, 18, 19, 16,

			20, 21, 22, 22, 23, 20
		};
		this->cubeVb = std::make_unique<VertexBuffer>(cube, sizeof(cube));

		this->cubeIb = std::make_unique<IndexBuffer>(indices, sizeof(indices)/sizeof(unsigned int));

		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(2);
		layout.Push<float>(3);
		this->cubeVao->AddBuffer(*this->cubeVb, layout);

		unsigned int uniformBlockIndexRed = glGetUniformBlockIndex(this->cubeShader->GetId(), "Matrices");
		glUniformBlockBinding(this->cubeShader->GetId(), uniformBlockIndexRed, 0);

		this->cubeTexture = std::make_unique<Texture>("res/textures/wall.jpg");
		this->cubeTexture2 = std::make_unique<Texture>("res/textures/grass.png");


		//����uniform������󣬶��ڶ����ɫ������ͬ��uniform����Ƶ�����ݡ�
		this->cubeUniformVb = std::make_unique<VertexBuffer>(nullptr, 2 * sizeof(glm::mat4), GL_UNIFORM_BUFFER);
		//this->cubeUniformVb->UnBind(GL_UNIFORM_BUFFER);
		this->cubeUniformVb->BindData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4));


		glm::mat4 projection = glm::perspective(glm::radians(this->camera.Zoom), (float)800 / (float)600, 0.1f, 100.0f);
		this->cubeUniformVb->Bind(GL_UNIFORM_BUFFER);
		GLCALL(glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection)));
		this->cubeUniformVb->UnBind(GL_UNIFORM_BUFFER);


		this->cubeShader->Bind();
		this->cubeShader->SetUniform1i("tex", 0);
		this->cubeShader->SetUniform1i("tex2", 1);
	}

	TestAdvancedLore::~TestAdvancedLore()
	{

	}

	void TestAdvancedLore::OnUpdate(float delta)
	{

	}

	void TestAdvancedLore::OnRender()
	{
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.25f, 0.24f, 0.24f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glm::mat4 projection = glm::perspective(glm::radians(this->camera.Zoom), (float)800 / (float)600, 0.1f, 100.0f);
		glm::mat4 view = glm::lookAt(this->camera.Position, this->camera.Position + this->camera.Front, this->camera.Up);
		
		this->cubeUniformVb->Bind(GL_UNIFORM_BUFFER);
		GLCALL(glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view)));
		this->cubeUniformVb->UnBind(GL_UNIFORM_BUFFER);

		glm::mat4 model = glm::mat4(1.0f);

		Render render;
		this->cubeVao->Bind();
		this->cubeShader->Bind();

		this->cubeShader->SetUniformMat4f("model", model);

		GLCALL(this->cubeTexture->Bind());
		this->cubeTexture2->Bind(1);

		GLCALL(render.Draw(*this->cubeVao, *this->cubeIb, *this->cubeShader));
	}

	void TestAdvancedLore::OnImGuiRender()
	{

	}

	void TestAdvancedLore::SetCamera(Camera& camera)
	{
		this->camera = camera;
	}

	std::string TestAdvancedLore::GetTestName() const
	{
		return "TestAdvancedLore";
	}
}