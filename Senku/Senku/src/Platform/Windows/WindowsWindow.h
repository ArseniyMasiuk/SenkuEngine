#pragma once
#include "SenkuEngine\Core\Window.h"
#include "SenkuEngine\Renderer\GraphicsContext.h"

#include <GLFW/glfw3.h>

namespace Senku
{


	class WindowsWindow : public Window
	{

	public:
		WindowsWindow(const WindowProps& props);
		~WindowsWindow() override;

	public:


		virtual void OnUpdate() override;

		virtual uint32_t GetWidth() const override;
		virtual uint32_t GetHeight() const override;

		virtual void SetVSync(bool enabled) override;
		virtual bool IsVSync() const override;

		virtual void* GetNativeWindow() const override;
	private:
		void Init(const WindowProps& props);
		void Shutdown();

		void SetEventsCallback();
	private:

		GLFWwindow* m_Window;

		Scope<GraphicsContext> m_GraphicsContext;

		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;

			EventsHandler& eventsHandler = EventsHandler::GetInstance();
		};

		WindowData m_Data;

	};
}

