#include "PrecompiledHeader.h"
#include <GL\glew.h>

#include "WindowsWindow.h"
#include "WindowsInput.h"

#include "SenkuEngine\Renderer\Renderer.h"

namespace Senku
{
	static void GLFWErrorCallback(int error, const char* description)
	{
		LOG_ERROR("GLFW Error: {0}. Description: {1}", error, description);
	}

	WindowsWindow::WindowsWindow(const WindowProps & props)
	{
		Init(props);
	}
	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
		// probably should be somewhere else
		glfwTerminate();
	}

	void WindowsWindow::OnUpdate()
	{
		/* Poll for and process events */
		glfwPollEvents();

		/* Swap front and back buffers */
		m_GraphicsContext->SwapBuffers();
	}
	uint32_t WindowsWindow::GetWidth() const
	{
		return m_Data.Width;
	}
	uint32_t WindowsWindow::GetHeight() const
	{
		return m_Data.Height;
	}
	void WindowsWindow::SetVSync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);
		m_Data.VSync = enabled;
	}
	bool WindowsWindow::IsVSync() const
	{
		return m_Data.VSync;
	}
	void * WindowsWindow::GetNativeWindow() const
	{
		return m_Window;
	}
	void WindowsWindow::Init(const WindowProps & props)
	{
		LOG_INFO("Init Window");
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;
		m_Data.eventsHandler.GetInstance();

		if (!glfwInit())
		{
			LOG_ERROR("Cant inicialize GLFW");
			return;
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0); // version of opengl
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // enabling core profile means that we have to create vertex array object by ourselves

		//glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

		/* Create a windowed mode window and its OpenGL context */
		m_Window = glfwCreateWindow(props.Width, props.Height, props.Title.c_str(), nullptr, nullptr);
		if (m_Window == NULL)
		{
			LOG_ERROR("Failed to create GLFW window");
			glfwTerminate();
		}


		m_GraphicsContext = GraphicsContext::Create(m_Window);
		m_GraphicsContext->Init();

		Renderer::Init();


		// todo: where exactly view port should be seted in initial load???
		// Renderer:: view port
		//glViewport(0, 0, m_Data.Width, m_Data.Height);

		//glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
		glfwSwapInterval(1); // vertical synchronization, investigate later what it is and how it works

		// make open gl function accessible
		if (glewInit() != GLEW_OK)
		{
			LOG_ERROR("[ERROR] Cant init glew");
			return;
		}


		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);

		// creating callbacks to recieve window events (key pressed-released-typed, mouse events window events etc...)

		//glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


		SetEventsCallback();
	}

	void WindowsWindow::Shutdown()
	{
		LOG_INFO("Destroying window");
		glfwDestroyWindow(m_Window);
	}
	void WindowsWindow::SetEventsCallback()
	{
		LOG_INFO("Setting up callbacks to GLFW in Window class");
		glfwSetErrorCallback(GLFWErrorCallback);

		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.Width = width;
			data.Height = height;

			WindowResizeEvent event(width, height);
			data.eventsHandler.PublishEvent(event);
		});


		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			data.eventsHandler.PublishEvent(event);
		});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
			case GLFW_PRESS:
			{
				KeyPressedEvent event(key, 0);
				data.eventsHandler.PublishEvent(event);
				break;
			}
			case GLFW_RELEASE:
			{
				KeyReleasedEvent event(key);
				data.eventsHandler.PublishEvent(event);
				break;
			}
			case GLFW_REPEAT:
			{
				KeyPressedEvent event(key, 1);
				data.eventsHandler.PublishEvent(event);
				break;
			}
			}
		});

		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			KeyTypedEvent event(keycode);
			data.eventsHandler.PublishEvent(event);
		});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
			case GLFW_PRESS:
			{
				MouseButtonPressedEvent event(button);
				data.eventsHandler.PublishEvent(event);
				break;
			}
			case GLFW_RELEASE:
			{
				MouseButtonReleasedEvent event(button);
				data.eventsHandler.PublishEvent(event);
				break;
			}
			}
		});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent event((float)xOffset, (float)yOffset);
			data.eventsHandler.PublishEvent(event);
		});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseMovedEvent event((float)xPos, (float)yPos);
			data.eventsHandler.PublishEvent(event);
		});
	}
}