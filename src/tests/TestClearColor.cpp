#include "tests/TestClearColor.h"
#include "Render.h"
#include "imgui/imgui.h"
namespace test
{
	TestClearColor::TestClearColor() :
		m_color{ 0.2f, 0.3f, 0.3f, 1.0f }
	{

	}

	TestClearColor::~TestClearColor()
	{

	}

	void TestClearColor::OnUpdate(float deltatime)
	{

	}

	void TestClearColor::OnRender()
	{
		GLCALL(glClearColor(m_color[0], m_color[1], m_color[2], m_color[3]));
		GLCALL(glClear(GL_COLOR_BUFFER_BIT));

	}

	void TestClearColor::OnImGuiRender()
	{
		ImGui::ColorEdit4("Clear Color", this->m_color);
	}

	std::string TestClearColor::GetTestName()
	{
		return "TestClearColor";
	}

}