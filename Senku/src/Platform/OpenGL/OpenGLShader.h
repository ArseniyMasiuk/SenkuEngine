#pragma once
#include "SenkuEngine\Renderer\Shader.h"
#include "SenkuEngine\Core\Base.h"

#include "DebugAsist.h"

#include <GL\glew.h>


namespace Senku
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string & filepath);
		OpenGLShader(const std::string & name, const std::string & vertexSource, const std::string & fragmentSource);
		~OpenGLShader();


		virtual void Bind() const override;
		virtual void UnBind() const override;

		virtual const std::string& GetName() override;
	private:

		//events
		void HandleEvent(Event& e);

	public:

		// set uniform
		//virtual void setUniformMat4(const std::string& name, const glm::mat4 & matrix) override;
		virtual void setUniform4f(const std::string& name, float v1, float v2, float v3, float v4) override;
		virtual void setUniform1i(const std::string& name, int val) override;
		//virtual void setUniform3fv(const std::string& name, glm::vec3 val) override;
		virtual void setUniform1f(const std::string& name, float val) override;

	private:

		int GetUniformLocation(const std::string& name);

		struct ShaderPrograSource
		{
			std::string vertexSource;
			std::string fragmentSource;
		};

		unsigned int compileShader(unsigned int type, const std::string &source);
		unsigned int createShader(const std::string & vertexShader, const std::string& fragmentShader);
		ShaderPrograSource parseShader(const std::string& filepath);

	private:
		std::string m_Name;

		std::string filepath;
		unsigned int m_rendererID = 0;

		std::unordered_map<std::string, int> m_UniformLocationCache;

	};
}