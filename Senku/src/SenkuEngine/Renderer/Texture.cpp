#include "PrecompiledHeader.h"

#include "Texture.h"
#include "Renderer.h"
#include "Platform\OpenGL\OpenGLTexture.h"

namespace Senku
{
	Ref<Texture2D> Texture2D::Create(const std::string & path)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None: ASSERT(false) return nullptr;
		case RendererAPI::API::OpenGL: return CreateRef<OpenGLTexture2D>(path);
		}

		ASSERT(false);
		return nullptr;
		
	}
}