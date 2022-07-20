#pragma once
#include "Base.h"
#include "Window.h"

int main(int argc, char** argv);

namespace Senku
{
	class Application
	{
	public:
		Application();
		~Application();


		void Close();

		static std::shared_ptr <Application> Get() { return s_AppInstance; }

	private:
		// events

	private:

		void Run();

	private:
		//members
		bool m_Running = true;
		Scope<Window> m_Window;

	private:

		static std::shared_ptr <Application> s_AppInstance;
		friend int ::main(int argc, char** argv);

	};

}