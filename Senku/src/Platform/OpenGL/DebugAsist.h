#pragma once

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


}