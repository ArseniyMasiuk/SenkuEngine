#include "PrecompiledHeader.h"
#include "OpenGLShader.h"



namespace Senku
{
	OpenGLShader::OpenGLShader(const std::string & filepath)
	{
		{
			// Extract name from filepath
			auto lastSlash = filepath.find_last_of("/\\");
			lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
			auto lastDot = filepath.rfind('.');
			auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
			m_Name = filepath.substr(lastSlash, count);

		}

		ShaderPrograSource source = parseShader(filepath);

		m_rendererID = createShader(source.vertexSource, source.fragmentSource);
		//glUseProgram(m_rendererID);

		EventsHandler::GetInstance().SubscribeForEvent(EventCategory(EventCategory::EventCategoryKeyboard | EventCategory::EventCategoryInput), BIND_EVENT_FN(OpenGLShader::HandleEvent));


	}
	OpenGLShader::OpenGLShader(const std::string & name, const std::string & vertexSource, const std::string & fragmentSource)
	{
		m_Name = name;

		m_rendererID = createShader(vertexSource, fragmentSource);
		//glUseProgram(m_rendererID);

		EventsHandler::GetInstance().SubscribeForEvent(EventCategory(EventCategory::EventCategoryKeyboard | EventCategory::EventCategoryInput), BIND_EVENT_FN(OpenGLShader::HandleEvent));
	}
	OpenGLShader::~OpenGLShader()
	{
		GLCall(glDeleteProgram(m_rendererID));
	}
	void OpenGLShader::Bind() const
	{
		GLCall(glUseProgram(m_rendererID));
	}
	void OpenGLShader::UnBind() const
	{
		GLCall(glUseProgram(0));
	}
	const std::string & OpenGLShader::GetName()
	{
		return m_Name;
	}
	void OpenGLShader::HandleEvent(Event & e)
	{
		if (e.GetEventType() == EventType::KeyPressed)
		{
			KeyPressedEvent& kEvent = dynamic_cast<KeyPressedEvent&>(e);

			if (kEvent.GetKeyCode() == Key::R)
				setUniform4f("u_Color", 1.0f, 0.0f, 0.0f, 0.1f);

			if (kEvent.GetKeyCode() == Key::G)
				setUniform4f("u_Color", 0.0f, 1.0f, 0.0f, 0.1f);

			if (kEvent.GetKeyCode() == Key::B)
				setUniform4f("u_Color", 0.0f, 0.0f, 1.0f, 0.1f);
		}
	}
	void OpenGLShader::setUniform4f(const std::string & name, float v1, float v2, float v3, float v4)
	{
		GLCall(glUniform4f(GetUniformLocation(name.c_str()), v1, v2, v3, v4));
	}
	void OpenGLShader::setUniform1i(const std::string & name, int val)
	{
		GLCall(glUniform1i(GetUniformLocation(name.c_str()), val));
	}
	void OpenGLShader::setUniform1f(const std::string & name, float val)
	{
		GLCall(glUniform1f(GetUniformLocation(name.c_str()), val));
	}

	//void OpenGLShader::setUniformMat4(const std::string & name, const glm::mat4 & matrix)
	//{
	//	/*
	//	int modelLoc = glGetUniformLocation(shaderLoader.getShader(), "model");
	//	ASSERT(modelLoc != -1);
	//	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	//	*/
	//	GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix)));
	//}

	//void OpenGLShader::setUniform3fv(const std::string & name, glm::vec3 val)
	//{
	//	GLCall(glUniform3fv(GetUniformLocation(name.c_str()), 1, glm::value_ptr(val)));
	//}

	int OpenGLShader::GetUniformLocation(const std::string & name)
	{
		if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
			return m_UniformLocationCache[name];

		GLCall(int location = glGetUniformLocation(m_rendererID, name.c_str()));

		if (location == -1)
		{
			ASSERT(false);
			LOG_ERROR("Uniform {0} does not exist", name);
		}

		m_UniformLocationCache[name] = location;

		return location;
	}
	unsigned int OpenGLShader::compileShader(unsigned int type, const std::string & source)
	{
		GLCall(unsigned int id = glCreateShader(type));
		const char* src = source.c_str();
		GLCall(glShaderSource(id, 1, &src, nullptr));
		GLCall(glCompileShader(id));

		//TODO: error handling
		int result;
		GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
		if (result == GL_FALSE)
		{
			int length;
			GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
			char *message = (char *)alloca(length * sizeof(char));
			GLCall(glGetShaderInfoLog(id, length, &length, message));

			GLCall(glDeleteShader(id));
			LOG_ERROR("Failed to compile {0} shader", (type == GL_VERTEX_SHADER ? " vertex" : " fragment"));
			LOG_ERROR("Error message : {0}", message);
			ASSERT(false);

			return 0;
		}

		return id;
	}
	unsigned int OpenGLShader::createShader(const std::string & vertexShader, const std::string & fragmentShader)
	{
		GLCall(unsigned int program = glCreateProgram());
		GLCall(unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader));
		GLCall(unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader));

		GLCall(glAttachShader(program, vs));
		GLCall(glAttachShader(program, fs));

		GLCall(glLinkProgram(program));

		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int *)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);
			// Don't leak shaders either.
			glDeleteShader(vs);
			glDeleteShader(fs);

			// Use the infoLog as you see fit.
			LOG_ERROR("Shader linking Error: {0}", infoLog.data());
			ASSERT(false);
			// In this simple program, we'll just leave
			return 0;
		}

		GLCall(glValidateProgram(program));


		GLCall(glDeleteShader(vs));
		GLCall(glDeleteShader(fs));

		return program;
	}
	OpenGLShader::ShaderPrograSource OpenGLShader::parseShader(const std::string & filepath)
	{
		std::ifstream stream(filepath);

		if (!stream.is_open())
		{
			LOG_ERROR("Shader file was not opened. FilePath: {0}", filepath);
			ASSERT(false);
		}

		enum class ShaderType
		{
			NONE = -1, VERTEX = 0, FRAGMENT = 1
		};

		std::string line;
		std::stringstream ss[2];
		ShaderType type = ShaderType::NONE;

		while (getline(stream, line))
		{
			if (line.find("#shader") != std::string::npos)
			{
				if (line.find("vertex") != std::string::npos)
					type = ShaderType::VERTEX;
				else if (line.find("fragment") != std::string::npos)
					type = ShaderType::FRAGMENT;
			}
			else
			{
				ss[(int)type] << line << "\n";
			}
		}

		return { ss[0].str(), ss[1].str() };
	}
}
