#include "PrecompiledHeader.h"
#include "Renderer.h"

namespace Senku
{
	void Renderer::Init()
	{
		RenderCommand::Init();
	}
	void Renderer::ShutDown()
	{
	}
	void Renderer::BeginScene()
	{
	}
	void Renderer::EndScene()
	{
	}
	void Renderer::WindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewPort(0, 0, width, height);
	}
	void Renderer::Submit(const Ref<VertexArray>& vertexArray)
	{
		RenderCommand::DrawIndexed(vertexArray);
	}
}
