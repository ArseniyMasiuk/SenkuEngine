#pragma once
#include "SenkuEngine\Renderer\FrameBuffer.h"

namespace Senku
{

	class OpenGLFrameBuffer : public FrameBuffer
	{
	public:
		OpenGLFrameBuffer(const FrameBufferSpecification& specification);
		~OpenGLFrameBuffer();

		virtual const FrameBufferSpecification& GetSpecification() const override;

		void Invalidate(); // means state is not valid so, recreate

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void Resize(uint32_t width, uint32_t height) override;

		virtual uint32_t GetRendererID() override { return m_RendererID; }

		virtual uint32_t GetColorAttachment() override { return m_ColorAttachment; }

	private:
		uint32_t m_RendererID;

		uint32_t m_ColorAttachment, m_DepthAttachment, m_RenderAttachment;

		FrameBufferSpecification m_Specification;
	};
}