#pragma once
#include "SenkuEngine\Renderer\Shader.h"
#include "SenkuEngine\Core\Base.h"


#include <GL\glew.h>


namespace Senku
{

	// should be moved to separated header to include it to all sources that use opengl 
#define ASSERT(x) if(!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x,__FILE__,__LINE__))


	static void GLClearError()
	{
		while (glGetError() != GL_NO_ERROR);
	}

	static bool GLLogCall(const char * function, const char * file, int line)
	{
		while (GLenum error = glGetError())
		{
			LOG_ERROR("OpenGL ERROR {0}, \nfunction: {1} \nfile: {2} \nline: {3}", error, function, file, line);
			//std::cout << "[OpenGl Error] (" << error << ")" << "function: " << function
			//	<< " file: " << file << " line: " << line << std::endl;
			return false;
		}
		return true;
	}





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