#include "PrecompiledHeader.h"
#include "VertexArray.h"

#include "Platform\OpenGL\OpenGLVertexArray.h"

#include "Renderer.h"

namespace Senku
{

	Ref<VertexArray> VertexArray::Create()
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None: ASSERT(false) return nullptr;
		case RendererAPI::API::OpenGL: return CreateRef<OpenGLVertexArray>();
		}

		ASSERT(false);
		return nullptr;
	}
}
