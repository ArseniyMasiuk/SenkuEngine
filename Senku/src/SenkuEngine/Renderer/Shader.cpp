#include "PrecompiledHeader.h"
#include "Renderer.h"

#include "Shader.h"

#include "Platform\OpenGL\OpenGLShader.h"

namespace Senku
{
	Ref<Shader> Shader::Create(const std::string & filepath)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None: ASSERT(false) return nullptr;
		case RendererAPI::API::OpenGL: return CreateRef<OpenGLShader>(filepath);
		}

		ASSERT(false);
		return nullptr;

		
	}
	Ref<Shader> Shader::Create(const std::string & name, const std::string & vertexSource, const std::string & fragmentSource)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None: ASSERT(false) return nullptr;
		case RendererAPI::API::OpenGL: return CreateRef<OpenGLShader>(name, vertexSource, fragmentSource);
		}

		ASSERT(false);
		return nullptr;
		
	}
}