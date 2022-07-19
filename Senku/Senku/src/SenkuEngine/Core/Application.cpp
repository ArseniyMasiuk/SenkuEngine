#include "PrecompiledHeader.h"
#include "Application.h"

namespace Senku
{
	Application* Application::s_AppInstance = nullptr;
	Application::Application()
	{
		s_AppInstance = this;

		// todo: create window

		// create graphic context, only OpenGL but i will pretent that there would be another so will add apstractions
	}

	Application::~Application()
	{
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
	// todo: init logger 

	// todo: creating app like in hazel engine

	Senku::Application app;



	// run app
	app.Run();


	system("pause");
	return 0;

}