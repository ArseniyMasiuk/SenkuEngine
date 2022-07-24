#pragma once

#include "VertexArray.h"
#include "glm\glm.hpp"

namespace Senku
{
	class RendererAPI
	{
	public:
		enum class API
		{
			None, OpenGL
		};

	public:

		virtual void Init() = 0;

		virtual void Clear() = 0;
		virtual void ClearColor(const glm::vec4& color) = 0;
		virtual void SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) = 0;


		static Scope<RendererAPI> Create();

		inline static API GetAPI() { return s_RendererAPI; }
	private:

		static API s_RendererAPI;
	};
}