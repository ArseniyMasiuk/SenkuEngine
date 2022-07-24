#include "PrecompiledHeader.h"
#include "RenderCommand.h"

#include "Platform\OpenGL\OpenGLRendererAPI.h"

namespace Senku
{
	Scope<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();
}
