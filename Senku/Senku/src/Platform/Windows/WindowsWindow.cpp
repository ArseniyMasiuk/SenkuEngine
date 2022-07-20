#include "PrecompiledHeader.h"
#include <GL\glew.h>

#include "WindowsWindow.h"




namespace Senku
{

	WindowsWindow::WindowsWindow(const WindowProps & props)
	{
		Init(props);
	}
	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}
	void WindowsWindow::OnUpdate()
	{
	}
	uint32_t WindowsWindow::GetWidth() const
	{
		return uint32_t();
	}
	uint32_t WindowsWindow::GetHeight() const
	{
		return uint32_t();
	}
	void WindowsWindow::SetVSync(bool enabled)
	{
	}
	bool WindowsWindow::IsVSync() const
	{
		return false;
	}
	void * WindowsWindow::GetNativeWindow() const
	{
		return nullptr;
	}
	void WindowsWindow::Init(const WindowProps & props)
	{
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;
		//m_Data.eventsHandler.GetInstance();

		if (!glfwInit())
		{
			LOG_ERROR("Cant inicialize GLFW");
			return;
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0); // version of opengl
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // enabling core profile means that we have to create vertex array object by ourselves


		/* Create a windowed mode window and its OpenGL context */
		m_Window = glfwCreateWindow(props.Width, props.Height, props.Title.c_str(), NULL, NULL);
		if (m_Window == NULL)
		{
			LOG_ERROR("Failed to create GLFW window");
			glfwTerminate();
		}

		/* Make the window's context current */
		glfwMakeContextCurrent(m_Window);
		//glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
		glfwSwapInterval(1); // vertical synchronization, investigate later what it is and how it works

		// make open gl function accessible
		if (glewInit() != GLEW_OK)
		{
			LOG_ERROR("[ERROR] Cant init glew");
			return;
		}


		// here is error spdlog\spdlog\fmt\bundled\core.h(1706): error C4996: 'fmt::v8::detail::arg_mapper<Context>::map': was declared deprecated
		// todo: fix it
		//LOG_INFO("Open GL Info:");
		//LOG_INFO("Vendor:   {0}", glGetString(GL_VENDOR));
		//LOG_INFO("Renderer: {0}", glGetString(GL_RENDERER));
		//LOG_INFO("Version:  {0}", glGetString(GL_VERSION));

		glViewport(0, 0, m_Data.Width, m_Data.Height);

		// configure global opengl state
		// -----------------------------
		glEnable(GL_DEPTH_TEST);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(false);

		// creating callbacks to recieve window events (key pressed-released-typed, mouse events window events etc...)
		//SetEventsCallback();

		glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


		glEnable(GL_DEBUG_OUTPUT);

		//glDebugMessageCallback(MessageCallback, 0);

		//creating hooks to recieve events
		//basicaly with firs call we create it 
		//SubsciberInfo info(&WindowGLFW::ProcessEvents, this);

		//EventsHandler::GetInstance().SubscribeForEvent(EventCategory::EventCategoryApplication, BIND_EVENT_FN(WindowGLFW::ProcessEventWindowResize));
		//EventsHandler::GetInstance().SubscribeForEvent(EventCategory(EventCategory::EventCategoryKeyboard | EventCategory::EventCategoryInput), BIND_EVENT_FN(WindowGLFW::ProcessEventKeyboardEvent));
	}

	void WindowsWindow::Shutdown()
	{
		LOG_INFO("Destroying window");
		glfwTerminate();
	}
}