#include "PrecompiledHeader.h"
#include "Renderer.h"


namespace Senku
{
	Scope<Renderer::SceneData> Renderer::s_SceneData = CreateScope<Renderer::SceneData>();


	void Renderer::Init()
	{
		RenderCommand::Init();
	}
	void Renderer::ShutDown()
	{
	}
	void Renderer::BeginScene(const Camera& camera)
	{
		s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}
	void Renderer::EndScene()
	{
	}
	void Renderer::WindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewPort(0, 0, width, height);
	}
	void Renderer::Submit(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, const glm::mat4& transform /*= glm::mat4(1.0f)*/)
	{
		shader->Bind();
		shader->setUniformMat4("u_ViewProjMat", s_SceneData->ViewProjectionMatrix);
		shader->setUniformMat4("u_Model", transform);


		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}
