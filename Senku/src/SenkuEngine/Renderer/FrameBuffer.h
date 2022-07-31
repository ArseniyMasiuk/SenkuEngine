#pragma once
#include "SenkuEngine\Core\Base.h"


namespace Senku
{

	struct FrameBufferSpecification
	{
		uint32_t Width, Height;

		uint32_t Samples = 1;

		/* this basically says this framebuffer is suppose to be renderring to the swapshain
		eqvivalent in open gl to glBindFrameBuffer(0) - unbind all of my frame buffers i want to render to the screen
		allows to create a frame buffer that does not really exist

		will be used in "render pathes"?? - it has frame buffer as a target

		so to render to the screen set this walue "true"
		*/
		bool SwapChainTarget = false; 	
	};
	class FrameBuffer
	{
	public:

		virtual ~FrameBuffer() = default;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;
		virtual uint32_t GetRendererID() = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;

		virtual uint32_t GetColorAttachment() = 0;

		//virtual FrameBufferSpecification& GetSpecification() = 0;
		virtual const FrameBufferSpecification& GetSpecification() const = 0;

		static Ref<FrameBuffer> Create(const FrameBufferSpecification& specification);
	};
}