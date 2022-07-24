#pragma once

#include "SenkuEngine\Renderer\RendererAPI.h"

namespace Senku
{
	class OpenGLRendererAPI :public RendererAPI
	{
	public:

		virtual void Init() override;

		virtual void Clear() override;
		virtual void ClearColor(const glm::vec4& color) override;
		virtual void SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) override;



	};
}