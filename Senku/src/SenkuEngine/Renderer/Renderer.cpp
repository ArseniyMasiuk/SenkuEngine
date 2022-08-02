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
	void Renderer::BeginScene(const Ref<PerspectiveCamera>& camera)
	{
		s_SceneData->ViewProjectionMatrix = camera->GetViewProjectionMatrix();
		s_SceneData->m_CameraPosition = camera->GetPosition();
	}
	void Renderer::EndScene()
	{
	}
	void Renderer::WindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewPort(0, 0, width, height);
	}
	void Renderer::Submit(const Ref<MaterialInstance>& material, const Ref<VertexArray>& vertexArray, const glm::mat4& transform /*= glm::mat4(1.0f)*/)
	{
		material->Bind();
		const Ref<Shader> shader = material->GetShader();
		shader->setUniformMat4("u_ViewProjMat", s_SceneData->ViewProjectionMatrix); // sould be done only once after binding it basicaly while rendering whole scene
		shader->setUniformMat4("u_Model", transform);


		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}
