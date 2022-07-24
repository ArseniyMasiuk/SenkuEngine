#include "PrecompiledHeader.h"
#include "Buffer.h"

#include "Platform\OpenGL\OpenGLBuffer.h"


namespace Senku
{
	Ref<VertexBuffer> VertexBuffer::Create(float * vertecies, uint32_t size)
	{
		return CreateRef<OpenGLVertexBuffer>(vertecies, size);
	}
	Ref<IndexBuffer> IndexBuffer::Create(uint32_t * indecies, uint32_t count)
	{
		return CreateRef<OpenGLIndexBuffer>(indecies, count);
	}
}