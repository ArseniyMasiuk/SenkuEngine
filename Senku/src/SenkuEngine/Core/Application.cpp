#include "PrecompiledHeader.h"
#include "Application.h"
#include "Log.h"
#include "Input.h"

#include "SenkuEngine\Renderer\Renderer.h"

namespace Senku
{
	Application* Application::s_AppInstance = nullptr;
	Application::Application(const WindowProps& prop)
	{
		LOG_INFO("Creating App");
		s_AppInstance = this;

		// todo: add ApplicationSpecification
		m_Window = Window::Create(prop);

		// create graphic context, only OpenGL but i will pretent that there would be another so will add abstractions

		//bind events 

		//creating hooks to recieve events
		//basicaly with firs call we create it 
		//SubsciberInfo info(&WindowGLFW::ProcessEvents, this);

		// todo: add some extra separation becasue resize events and close events has same event category but different event type
		// so they will be called twice for each event
		//EventsHandler::GetInstance().SubscribeForEvent(EventCategory::EventCategoryApplication, BIND_EVENT_FN(Application::OnWindowClose));
		//EventsHandler::GetInstance().SubscribeForEvent(EventCategory::EventCategoryApplication, BIND_EVENT_FN(Application::OnWindowResize));
		EventsHandler::GetInstance().SubscribeForEvent(EventCategory(EventCategory::EventCategoryKeyboard | EventCategory::EventCategoryInput), BIND_EVENT_FN(Application::OnKeyboardKeyPressed));
		EventsHandler::GetInstance().SubscribeForEvents(BIND_EVENT_FN(Application::OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay((Layer*)(m_ImGuiLayer));

	}

	Application::~Application()
	{
		LOG_INFO("Destroying App");
	}

	void Application::PushLayer(Layer * layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer * layer)
	{
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::Close()
	{
		m_Running = false;
	}

	void Application::OnEvent(Event & e)
	{
		//WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		if (e.GetEventType() == EventType::WindowClose)
			Close();


		// todo: change viewport resolution. There sould be renderer that handle it
		if (e.GetEventType() == EventType::WindowResize)
		{
			WindowResizeEvent& event = (WindowResizeEvent&)e;
			RenderCommand::SetViewPort(0, 0, event.GetWidth(), event.GetHeight());
		}

		// todo: pass events to layers

		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); it++)
		{
			if ((*it)->OnEvent(e))
				break;
		}

	}

	void Application::OnKeyboardKeyPressed(Event & e)
	{
		if (Input::IsKeyPressed(Key::Escape))
		{
			LOG_INFO("Escape pressed. Calling close function");
			Close();
		}
	}

	void Application::Run()
	{
		// some timer
		auto tp1 = std::chrono::system_clock::now();
		auto tp2 = std::chrono::system_clock::now();


		while (m_Running)
		{

			tp2 = std::chrono::system_clock::now();
			std::chrono::duration<float> elapsedTime = tp2 - tp1;
			tp1 = tp2;
			float fElapsedTime = elapsedTime.count();

			for (auto it = m_LayerStack.begin(); it != m_LayerStack.end(); it++)
			{
				(*it)->OnUpdate(fElapsedTime);
			}

			m_ImGuiLayer->Begin();
			for (auto it = m_LayerStack.begin(); it != m_LayerStack.end(); it++)
			{
				(*it)->OnImGuiRender();
			}
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}

}
