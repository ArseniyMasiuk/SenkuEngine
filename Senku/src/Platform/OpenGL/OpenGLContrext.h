#pragma once
#include "SenkuEngine\Renderer\GraphicsContext.h"

struct GLFWwindow;

namespace Senku
{
	class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* window);
		virtual void Init() override;
		virtual void SwapBuffers() override;

	private:
		 
		GLFWwindow* m_Window;

	};
}