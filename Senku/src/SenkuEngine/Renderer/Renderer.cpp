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
	void Renderer::BeginScene(const Ref<PerspectiveCamera>& camera, DirectLight dirLight)
	{
		s_SceneData->ViewProjectionMatrix = camera->GetViewProjectionMatrix();
		s_SceneData->m_CameraPosition = camera->GetPosition();
		s_SceneData->dirLight = dirLight;
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


		//setup direction light
		shader->setUniform3fv("dirLight.lightColor", s_SceneData->dirLight.m_LightColor);
		shader->setUniform3fv("dirLight.direction", s_SceneData->dirLight.m_Direction);
		shader->setUniform1f("dirLight.distance", s_SceneData->dirLight.m_Distance);
		shader->setUniform1f("dirLight.intencisty", s_SceneData->dirLight.m_Intensity);
		//shader->setUniform3fv("dirLight.ambient", s_SceneData->dirLight.m_Ambient);
		//shader->setUniform3fv("dirLight.diffuse", s_SceneData->dirLight.m_Diffuse);
		//shader->setUniform3fv("dirLight.specular", s_SceneData->dirLight.m_Specular);

		shader->setUniform3fv("u_CameraPosition", s_SceneData->m_CameraPosition);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}
