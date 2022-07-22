#include "PrecompiledHeader.h"
#include "Application.h"
#include "Log.h"
#include "Input.h"

// should be move to some rendere part
#include <GL\glew.h>


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

	}

	Application::~Application()
	{
		LOG_INFO("Destroying App");
	}

	void Application::PushLayer(Layer * layer)
	{
		m_LayerStack.PushLayer(layer);
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
			glViewport(0, 0, event.GetWidth(), event.GetHeight());
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
		while (m_Running)
		{
			glClearColor(0.1f, 1.0f, 0.0f, 0.0f);
			glClearDepth(1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			for (auto it = m_LayerStack.begin(); it != m_LayerStack.end(); it++)
			{
				(*it)->OnUpdate();
			}


			m_Window->OnUpdate();
		}
	}

}
