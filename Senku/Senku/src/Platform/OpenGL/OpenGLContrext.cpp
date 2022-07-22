#include "PrecompiledHeader.h"
#include "SenkuEngine\Core\Base.h"
#include "OpenGLContrext.h"

#include "Platform\OpenGL\OpenGLContrext.h"
#include <GL\glew.h>
#include <GLFW/glfw3.h>

namespace Senku
{
	OpenGLContext::OpenGLContext(GLFWwindow * window)
	{
		m_Window = window;
	}

	void OpenGLContext::Init()
	{
		/* Make the window's context current */
		glfwMakeContextCurrent(m_Window);

		//LOG_INFO("OpenGL Info:");
		//LOG_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
		//LOG_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
		//LOG_INFO("  Version: {0}", glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_Window);
	}

}