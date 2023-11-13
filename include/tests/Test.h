#pragma once
#include <functional>
#include <vector>
#include <string>
#include "Camera.h"
namespace test 
{
	class Test
	{
	public:
		Test() {}
		virtual ~Test() {}
		virtual void OnUpdate(float deltatime) {}
		virtual void OnRender() {}
		virtual void OnImGuiRender() {}
		virtual void SetCamera(Camera& camera) {}
		virtual std::string GetTestName() { return "Test"; }
		virtual Camera GetCamera() { return Camera(); }
	private:

	};

	class TestMenu :public Test
	{
	public:
		TestMenu(Test*& currentTestPointer);
		void OnImGuiRender() override;

		template<typename T>
		void RegisterTest(const std::string& testName);
	private:
		Test*& m_CurrentTest;
		std::vector<std::pair<std::string, std::function<Test*()>>> m_Tests;
	};

	template<typename T>
	inline void TestMenu::RegisterTest(const std::string& testName)
	{
		//std::cout << "Register test: " << testName << std::endl;

		m_Tests.push_back(std::make_pair(testName, []() {return new T(); }));
	}

};