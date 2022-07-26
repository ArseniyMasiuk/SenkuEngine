#pragma once

#include "RenderCommand.h"
#include "Camera.h"
#include "Shader.h"

namespace Senku
{

	class Renderer
	{
	public:
		static void Init();
		static void ShutDown();
		static void BeginScene(const Ref<PerspectiveCamera>& camera); // todo: will take all data that need for scene eg camera, shader, and so on
		static void EndScene();

		static void WindowResize(uint32_t width, uint32_t height);


		static void Submit(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, const glm::mat4& transform = glm::mat4(1.0f));

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private: 
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static Scope<SceneData> s_SceneData;
	};
}