#include "PrecompiledHeader.h"
#include "VertexArray.h"

#include "Platform\OpenGL\OpenGLVertexArray.h"

namespace Senku
{

	Ref<VertexArray> VertexArray::Create()
	{
		return CreateRef<OpenGLVertexArray>();
	}
}
