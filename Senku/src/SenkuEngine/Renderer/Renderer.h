#pragma once

#include "RenderCommand.h"

namespace Senku
{

	class Renderer
	{
	public:
		static void Init();
		static void ShutDown();
		static void BeginScene(); // todo: will take all data that need for scene eg camera, shader, and so on
		static void EndScene();

		static void WindowResize(uint32_t width, uint32_t height);


		static void Submit(const Ref<VertexArray>& vertexArray);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	};
}