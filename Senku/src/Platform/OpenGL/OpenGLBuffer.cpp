#include "PrecompiledHeader.h"
#include "OpenGLBuffer.h"
#include "SenkuEngine\Core\Base.h"

#include "DebugAsist.h"


namespace Senku
{
	OpenGLVertexBuffer::OpenGLVertexBuffer(float * vertecies, uint32_t size)
	{
		//unsigned int buffer;
		GLCall(glGenBuffers(1, &m_RendererID));
		//seletc or bind buffer
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
		GLCall(glBufferData(GL_ARRAY_BUFFER, size/*size of array*/, vertecies/*array*/, GL_STATIC_DRAW));
		//LOG_INFO("Creating Vertex Buffer. ID = {0}", m_RendererID);
	}
	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		//LOG_INFO("Deleting Vertex Buffer. ID = {0}", m_RendererID);
		GLCall(glDeleteBuffers(1, &m_RendererID));
	}
	void OpenGLVertexBuffer::Bind() const
	{
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
	}
	void OpenGLVertexBuffer::Unbind() const
	{
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}
	/*===============================================================================================================================*/


	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indecies, uint32_t count)
		:m_Count(count)
	{
		ASSERT(sizeof(unsigned int) == sizeof(GLuint));

		GLCall(glGenBuffers(1, &m_RendererID));
		//seletc or bind buffer
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indecies/*array*/, GL_STATIC_DRAW));

		//LOG_INFO("Creating Index Buffer. ID = {0}", m_RendererID);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		//LOG_INFO("Deleting Index Buffer. ID = {0}", m_RendererID);
		GLCall(glDeleteBuffers(1, &m_RendererID));
	}

	void OpenGLIndexBuffer::Bind() const
	{
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
	}

	void OpenGLIndexBuffer::Unbind() const
	{
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}

}
