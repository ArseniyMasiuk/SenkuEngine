#pragma once
#include "Base.h"
#include "Window.h"
#include "LayerStack.h"
#include "SenkuEngine\ImGui\ImGuiLayer.h"

int main(int argc, char** argv);

namespace Senku
{
	class Application
	{
	public:
		Application(const WindowProps& prop);
		~Application();

	public:
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		void Close();

		static Application* Get() { return s_AppInstance; }
		Window& GetWindow() { return *m_Window; }
	private:
		// events
		void OnEvent(Event& e);
		void OnKeyboardKeyPressed(Event& e);

	private:

		void Run();

	private:
		//members
		bool m_Running = true;
		Scope<Window> m_Window;

		LayerStack m_LayerStack;

		ImGuiLayer* m_ImGuiLayer;

	private:

		static Application* s_AppInstance;
		friend int ::main(int argc, char** argv);

	};

	// will add later some properties
	// should be created in user scope
	Application* CreateApplication();
}