#include "PrecompiledHeader.h"
#include "RendererAPI.h"


#include "Platform\OpenGL\OpenGLRendererAPI.h"
namespace Senku
{
	RendererAPI::API RendererAPI::s_RendererAPI = RendererAPI::API::OpenGL;


	Scope<RendererAPI> RendererAPI::Create()
	{
		switch (s_RendererAPI)
		{
		case RendererAPI::API::None: ASSERT(false) return nullptr;
		case RendererAPI::API::OpenGL: return CreateScope<OpenGLRendererAPI>();
		}

		ASSERT(false);
		return nullptr;
	}

}