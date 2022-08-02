#pragma once

#include "RenderCommand.h"
#include "Camera.h"
#include "Shader.h"
#include "Material.h"
#include "Light.h"

namespace Senku
{

	class Renderer
	{
	public:
		static void Init();
		static void ShutDown();
		static void BeginScene(const Ref<PerspectiveCamera>& camera, DirectLight dirLight); // todo: will take all data that need for scene eg camera, shader, and so on
		static void EndScene();

		static void WindowResize(uint32_t width, uint32_t height);


		static void Submit(const Ref<MaterialInstance>& material, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private: 
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
			glm::vec3 m_CameraPosition;
			DirectLight dirLight;
		};

		static Scope<SceneData> s_SceneData;
	};
}