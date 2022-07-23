#include "PrecompiledHeader.h"

#include "Shader.h"

#include "Platform\OpenGL\OpenGLShader.h"


namespace Senku
{
	Ref<Shader> Shader::Create(const std::string & filepath)
	{
		return CreateRef<OpenGLShader>(filepath);
	}
	Ref<Shader> Shader::Create(const std::string & name, const std::string & vertexSource, const std::string & fragmentSource)
	{
		return CreateRef<OpenGLShader>(name, vertexSource, fragmentSource);
	}
}