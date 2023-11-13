#pragma once
#include "Test.h"
namespace test
{
	class TestClearColor :
		public Test
	{
	public:

		TestClearColor();
		~TestClearColor();
		void OnUpdate(float deltatime) override;
		void OnRender() override;
		void OnImGuiRender() override;
		std::string GetTestName() override;
	private:
		float m_color[4];
	};

}

