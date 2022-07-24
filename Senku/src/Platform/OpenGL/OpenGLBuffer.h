#pragma once
#include "SenkuEngine\Renderer\Buffer.h"

namespace Senku
{
	class OpenGLVertexBuffer: public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(float * vertecies, uint32_t size);
		virtual ~OpenGLVertexBuffer() override;

		virtual void Bind() const override;
		virtual void Unbind() const override;

	public:

		unsigned int m_RendererID;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t* indecies, uint32_t count);
		virtual ~OpenGLIndexBuffer() override;

		virtual void Bind() const override;
		virtual void Unbind() const override;


	public:

		// todo:: add count var
		unsigned int m_RendererID;
	};
}
