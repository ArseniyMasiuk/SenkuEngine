#include "PrecompiledHeader.h"
#include "FrameBuffer.h"

#include "Platform\OpenGL\OpenGLFrameBuffer.h"
#include "SenkuEngine\Renderer\Renderer.h"

namespace Senku
{
	Ref<FrameBuffer> FrameBuffer::Create(const FrameBufferSpecification & specification)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None: ASSERT(false) return nullptr;
		case RendererAPI::API::OpenGL: return CreateRef<OpenGLFrameBuffer>(specification);
		}

		ASSERT(false);
		return nullptr;
	}
}