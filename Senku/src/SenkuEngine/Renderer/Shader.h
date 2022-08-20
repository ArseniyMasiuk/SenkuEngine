#pragma once
#include "SenkuEngine\Core\Base.h"
#include "glm\glm.hpp"

namespace Senku
{
	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		virtual const std::string& GetName() = 0;
		virtual void ReloadShader(const std::string& path) = 0;

	public:

		// set uniform
		virtual void setUniformMat4(const std::string& name, const glm::mat4 & matrix) = 0;
		virtual void setUniform4f(const std::string& name, float v1, float v2, float v3, float v4) = 0;
		virtual void setUniform1i(const std::string& name, int val) = 0;
		virtual void setUniform3fv(const std::string& name, glm::vec3 val) = 0;
		virtual void setUniform1f(const std::string& name, float val) = 0;

		static Ref<Shader> Create(const std::string& filepath);
		static Ref<Shader> Create(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource);

	};
}