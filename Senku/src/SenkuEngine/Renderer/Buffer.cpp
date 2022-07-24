#include "PrecompiledHeader.h"
#include "Buffer.h"

#include "Platform\OpenGL\OpenGLBuffer.h"
#include "Renderer.h"

namespace Senku
{
	Ref<VertexBuffer> VertexBuffer::Create(float * vertecies, uint32_t size)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None: ASSERT(false) return nullptr;
		case RendererAPI::API::OpenGL: return CreateRef<OpenGLVertexBuffer>(vertecies, size);
		}

		ASSERT(false);
		return nullptr;
	}
	Ref<IndexBuffer> IndexBuffer::Create(uint32_t * indecies, uint32_t count)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None: ASSERT(false) return nullptr;
		case RendererAPI::API::OpenGL: return CreateRef<OpenGLIndexBuffer>(indecies, count);
		}

		ASSERT(false);
		return nullptr;
		
	}
}