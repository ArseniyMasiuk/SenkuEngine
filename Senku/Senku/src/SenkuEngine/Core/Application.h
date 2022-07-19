#pragma once


int main(int argc, char** argv);

namespace Senku
{
	class Application
	{
	public:
		Application();
		~Application();


	private:
		// events

	private:

		void Run();

	private:
		//members
		bool m_Running = true;

	private:

		static Application* s_AppInstance;
		friend int ::main(int argc, char** argv);

	};

}