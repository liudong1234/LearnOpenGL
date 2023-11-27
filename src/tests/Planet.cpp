#include "tests/Planet.h"
#include "VertexBufferLayout.h"
#include "glm/gtc/type_ptr.hpp"
#include "imgui/imgui.h"
#include <GLFW/glfw3.h>

namespace test
{
	Planet::Planet() :
		camera(glm::vec3(0.0f, 0.0f, 55.0f)),
		planet("res/model/planet/scene.obj"),
		satellite("res/model/rock/scene.obj")
	{
		std::string basepath = "res/shaders/" + this->GetTestName();
		std::string str1 = basepath + "/planet.shader";
		std::string str2 = basepath + "/PlanetAdvance.shader";
		this->planetShader = std::make_unique<Shader>(str1.c_str());
		this->advanceShader = std::make_unique<Shader>(str2.c_str());

		// generate a large list of semi-random model transformation matrices
// ------------------------------------------------------------------
		unsigned int amount = 5000;
		modelMatrices = new glm::mat4[amount];
		srand(static_cast<unsigned int>(glfwGetTime())); // initialize random seed
		float radius = 50.0;
		float offset = 2.5f;
		for (unsigned int i = 0; i < amount; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			// 1. translation: displace along circle with 'radius' in range [-offset, offset]
			float angle = (float)i / (float)amount * 360.0f;
			float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
			float x = sin(angle) * radius + displacement;
			displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
			float y = displacement * 0.4f; // keep height of asteroid field smaller compared to width of x and z
			displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
			float z = cos(angle) * radius + displacement;
			model = glm::translate(model, glm::vec3(x, y, z));

			// 2. scale: Scale between 0.05 and 0.25f
			float scale = static_cast<float>((rand() % 20) / 100.0 + 0.05);
			model = glm::scale(model, glm::vec3(scale));

			// 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
			float rotAngle = static_cast<float>((rand() % 360));
			model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

			// 4. now add to list of matrices
			modelMatrices[i] = model;
		}
		this->vb = std::make_unique<VertexBuffer>(modelMatrices, amount * sizeof(glm::mat4));
		
		for (unsigned int i = 0; i < this->satellite.meshes.size(); i++)
		{
			this->satellite.meshes[i].GetVertexArray()->Bind();//����λ��1,2�����ԣ���mesh�Ѿ���
			//������ʵ������Ĵ��룬�붥�������Ƿֿ���
			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
			glEnableVertexAttribArray(4);
			glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
			glEnableVertexAttribArray(5);
			glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
			glEnableVertexAttribArray(6);
			glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

			glVertexAttribDivisor(3, 1);// ����opengl����һ��ʵ��������
			glVertexAttribDivisor(4, 1);
			glVertexAttribDivisor(5, 1);
			glVertexAttribDivisor(6, 1);
			this->satellite.meshes[i].GetVertexArray()->UnBind();
			this->vVa.push_back(std::move(this->satellite.meshes[i].GetVertexArray()));
		}
	}

	Planet::~Planet()
	{

	}

	void Planet::OnUpdate(float delta)
	{

	}

	void Planet::OnRender()
	{
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.25f, 0.24f, 0.24f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// configure transformation matrices
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)800 / (float)600, 0.1f, 1000.0f);
		glm::mat4 view = camera.GetViewMatrix();;
		
		this->advanceShader->Bind();
		this->advanceShader->SetUniformMat4f("projection", projection);
		this->advanceShader->SetUniformMat4f("view", view);
		
		this->planetShader->Bind();
		this->planetShader->SetUniformMat4f("projection", projection);
		this->planetShader->SetUniformMat4f("view", view);

		// draw planet
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		this->planetShader->SetUniformMat4f("model", model);
		planet.Draw(*this->planetShader);

		unsigned int amount = 5000;
		// draw meteorites
		this->advanceShader->Bind();
		this->advanceShader->SetUniform1i("material.texture_diffuse1", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, satellite.texturesLoaded[0].id);
		for (unsigned int i = 0; i < this->vVa.size(); i++)
		{
			//this->planetShader->SetUniformMat4f("model", modelMatrices[i]);
			//this->satellite.Draw(*this->planetShader);
			this->vVa[i]->Bind();
			glDrawElementsInstanced(GL_TRIANGLES, static_cast<unsigned int>(this->satellite.meshes[i].indices.size()), GL_UNSIGNED_INT, 0, amount);
		}
	}

	void Planet::OnImGuiRender()
	{
		//ImGui::DragFloat3();
		ImGui::Text("application %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}

	void Planet::SetCamera(Camera& camera)
	{
		this->camera = camera;
	}

	std::string Planet::GetTestName() const
	{
		return "Planet";
	}
}