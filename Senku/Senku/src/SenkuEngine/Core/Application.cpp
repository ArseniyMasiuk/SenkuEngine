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
		EventsHandler::GetInstance().SubscribeForEvent(EventCategory::EventCategoryApplication, BIND_EVENT_FN(Application::OnWindowClose));
		EventsHandler::GetInstance().SubscribeForEvent(EventCategory::EventCategoryApplication, BIND_EVENT_FN(Application::OnWindowResize));
		EventsHandler::GetInstance().SubscribeForEvent(EventCategory(EventCategory::EventCategoryKeyboard | EventCategory::EventCategoryInput), BIND_EVENT_FN(Application::OnKeyboardKeyPressed));

	}

	Application::~Application()
	{
		LOG_INFO("Destroying App");
	}

	void Application::Close()
	{
		m_Running = false;
	}

	void Application::OnWindowResize(Event & e)
	{
		if (e.GetEventType() == EventType::WindowResize)
		{
			WindowResizeEvent& event = (WindowResizeEvent&)e;
			//m_Window->m_Data.Width = event.GetWidth();
			//m_Data.Height = event.GetHeight();
			LOG_INFO("Window resize {0}x{1}", event.GetWidth(), event.GetHeight());

			// todo: should be moved to renderer
			glViewport(0, 0, event.GetWidth(), event.GetHeight());
		}
	}

	void Application::OnWindowClose(Event & e)
	{
		if (e.GetEventType() == EventType::WindowClose)
		{
			LOG_INFO("Close Event handled. \n Closing window ...");
			m_Running = false;
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


			m_Window->OnUpdate();
		}
	}

}
