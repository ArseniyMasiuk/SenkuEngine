#include "PrecompiledHeader.h"
#include "SenkuEngine\Core\Base.h"
#include "GraphicsContext.h"
#include "Platform\OpenGL\OpenGLContrext.h"
#include "Renderer.h"

namespace Senku
{

	Scope<GraphicsContext> GraphicsContext::Create(void * window)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None: ASSERT(false) return nullptr;
		case RendererAPI::API::OpenGL: return CreateScope<OpenGLContext>(static_cast<GLFWwindow*>(window));
		}

		ASSERT(false);
		return nullptr;
		
	}
}
