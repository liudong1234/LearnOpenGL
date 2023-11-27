#include "tests/TestIntance.h"
#include "VertexBufferLayout.h"
#include "glm/gtc/type_ptr.hpp"

namespace test
{
	TestIntance::TestIntance() :
		camera(glm::vec3(0.0f, 0.0f, 3.0f))
	{
		float quadVertices[] =
		{
			// λ��          // ��ɫ
			-0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
			 0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
			-0.05f, -0.05f,  0.0f, 0.0f, 1.0f,

			-0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
			 0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
			 0.05f,  0.05f,  0.0f, 1.0f, 1.0f
		};
		//������Щƫ��λ��
		glm::vec2 translations[100];
		int index = 0;
		float offset = 0.1f;
		for (int y = -10; y < 10; y += 2)
		{
			for (int x = -10; x < 10; x += 2)
			{
				glm::vec2 translation;
				translation.x = (float)x / 10.0f + offset;
				translation.y = (float)y / 10.0f + offset;
				translations[index++] = translation;
			}
		}
		// store instance data in an array buffer
		// --------------------------------------
		this->instanceVbo = std::make_unique<VertexBuffer>(translations, sizeof(glm::vec2) * 100);

		//����quad
		this->quadShader = std::make_unique<Shader>("res/shaders/TestInstance/instance.shader");
		this->quadVao = std::make_unique<VertexArray>();
		this->quadVb = std::make_unique<VertexBuffer>(quadVertices, sizeof(quadVertices));

		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(3);
		this->quadVao->AddBuffer(*this->quadVb, layout);
		//also set instance data
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, this->instanceVbo->GetId()); // this attribute comes from a different vertex buffer
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
		this->instanceVbo->UnBind();
		glVertexAttribDivisor(2, 1); // tell OpenGL this is an instanced vertex attribute.
	}

	TestIntance::~TestIntance()
	{

	}

	void TestIntance::OnUpdate(float delta)
	{

	}

	void TestIntance::OnRender()
	{
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.25f, 0.24f, 0.24f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		this->quadShader->Bind();
		this->quadVao->Bind();
		GLCALL(glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 100));
		this->quadVao->UnBind();
	}

	void TestIntance::OnImGuiRender()
	{

	}

	void TestIntance::SetCamera(Camera& camera)
	{
		this->camera = camera;
	}

	std::string TestIntance::GetTestName() const
	{
		return "TestIntance";
	}
}