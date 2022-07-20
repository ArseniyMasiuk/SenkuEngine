#include "PrecompiledHeader.h"
#include "Application.h"
#include "Log.h"

namespace Senku
{
	std::shared_ptr <Application> Application::s_AppInstance = nullptr;
	Application::Application()
	{
		LOG_INFO("Creating App");
		s_AppInstance.reset(this);

		// todo: add ApplicationSpecification
		m_Window = Window::Create(WindowProps("Sandbox app", 1200, 800));

		// create graphic context, only OpenGL but i will pretent that there would be another so will add abstractions
	}

	Application::~Application()
	{
		LOG_INFO("Destroying App");
	}

	void Application::Close()
	{
		m_Running = false;
	}

	void Application::Run()
	{
		while (m_Running)
		{

		}
	}

}











int main(int argc, char** argv)
{
	Senku::Log::Init();

	// todo: creating app like in hazel engine

	Senku::Application app;



	// run app
	app.Run();


	system("pause");
	return 0;

}