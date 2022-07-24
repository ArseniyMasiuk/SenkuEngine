#include "PrecompiledHeader.h"
#include "OpenGLRendererAPI.h"

#include "DebugAsist.h"
#include <GL\glew.h>

namespace Senku
{

	void OpenGLMessageCallback(
		unsigned source,
		unsigned type,
		unsigned id,
		unsigned severity,
		int length,
		const char* message,
		const void* userParam)
	{
		switch (severity)
		{
		case GL_DEBUG_SEVERITY_HIGH:         LOG_ERROR(message); return;
		case GL_DEBUG_SEVERITY_MEDIUM:       LOG_ERROR(message); return;
		case GL_DEBUG_SEVERITY_LOW:          LOG_WARN(message); return;
		case GL_DEBUG_SEVERITY_NOTIFICATION: LOG_TRACE(message); return;
		}

	}

	void OpenGLRendererAPI::Init()
	{
		{
			// todo: investigae why error occurs
			// todo: add ifdef debug
			//glEnable(GL_DEBUG_OUTPUT);
			//glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			//glDebugMessageCallback(OpenGLMessageCallback, nullptr);

			//glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
		}

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LINE_SMOOTH);

	}
	void OpenGLRendererAPI::Clear()
	{
		GLCall(glClearDepth(1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	}
	void OpenGLRendererAPI::ClearColor(const glm::vec4 & color)
	{
		GLCall(glClearColor(color.r, color.g, color.b, color.a));
	}
	void OpenGLRendererAPI::SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}
	void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray)
	{
		vertexArray->Bind();
		GLCall(glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr));
	}
}
