#include "PrecompiledHeader.h"
#include "SandboxLayer.h"

#include "SenkuEngine\Renderer\Renderer.h"
#include "glm\ext\quaternion_transform.hpp"
#include "glm\ext\matrix_transform.hpp"


bool loadShape(std::string path, std::vector<float>& positions, std::vector<uint32_t>& indexies);

SandBoxLayer::SandBoxLayer()
{

	float width = static_cast<float>(Senku::Application::Get()->GetWindow().GetWidth());
	float height = static_cast<float>(Senku::Application::Get()->GetWindow().GetHeight());
	m_Camera = Senku::CreateRef<Senku::PerspectiveCamera>(45.0f, width / height, 0.01f, 10000.0f);

	m_Camera->SetPosition(glm::vec3(0, 0, -5));


	Senku::ModelLoader loader;
	//loader.LoadModel("Sandbox/assets/meshes/cube/untitled.obj");
	loader.LoadModel("Sandbox/assets/meshes/Cerberus_by_Andrew_Maximov/Cerberus_LP.FBX");

	Senku::Model model = loader.GetModels()[0];

	m_VertexArray = Senku::VertexArray::Create();

	Senku::Ref<Senku::VertexBuffer> vertexBuffer;
	vertexBuffer = Senku::VertexBuffer::Create((float*)(model.vertecies.data()), model.vertecies.size() * sizeof(Senku::Vertex));

	vertexBuffer->Bind();
	{
		Senku::BufferLayout layout =
		{
			{Senku::ShaderDataType::Float3, "aPos"},
			{Senku::ShaderDataType::Float3, "aNormal"},
			{Senku::ShaderDataType::Float2, "aTexCoord"}
		};
		vertexBuffer->SetLayout(layout);
	}
	m_VertexArray->AddVertexBuffer(vertexBuffer);

	Senku::Ref<Senku::IndexBuffer> indexBuffer;
	indexBuffer = Senku::IndexBuffer::Create(model.indices.data(), model.indices.size());


	m_VertexArray->SetIndexBuffer(indexBuffer);


	m_Shader = Senku::Shader::Create("Sandbox/assets/shaders/basicShader.glsl");

	//texture = Senku::Texture2D::Create("Sandbox/assets/textures/default.jpg");
	texture = Senku::Texture2D::Create("Sandbox/assets/meshes/Cerberus_by_Andrew_Maximov/Textures/Cerberus_A.tga");


	m_Material = Senku::CreateRef<Senku::MaterialInstance>(m_Shader);
	m_Material->AddTexture(texture, Senku::MaterialInstance::TextureType::eAlbedo);
	m_Material->mlt = model.mlt;
}

void SandBoxLayer::OnAttach()
{
}

void SandBoxLayer::OnDetach()
{
}

void SandBoxLayer::OnUpdate(float timeStep)
{
	//LOG_TRACE("SandboxLayerUpdating");

	m_Camera->UpdateCameraPosition(timeStep);

	Senku::RenderCommand::ClearColor({ 0.1f, 0.2f, 0.2f, 0.0f });
	Senku::RenderCommand::Clear();

	Senku::Renderer::BeginScene(m_Camera); // todo:: set up cameras light shaders all stuff that should be common

	glm::mat4 model = glm::scale(glm::mat4(1), glm::vec3(0.1, 0.1, 0.1)) * glm::rotate(glm::mat4(1.0f), glm::radians(-90.f), glm::vec3(1, 0, 0));

	Senku::Renderer::Submit(m_Material, m_VertexArray, model);


	Senku::Renderer::EndScene();
	/*if (Senku::Input::IsKeyPressed(Senku::Key::A))
	{
		m_Shader->setUniform4f("u_Color", 0.1f, 0.7f, 0.4f, 0.8f);
	}*/

}

bool SandBoxLayer::OnEvent(Senku::Event & event)
{

	if (Senku::Input::IsKeyPressed(Senku::Key::D1))
		m_Material->mlt.baseColor = glm::vec3(1.0f, 0.0f, 0.0f);


	if (Senku::Input::IsKeyPressed(Senku::Key::D2))
		m_Material->mlt.baseColor = glm::vec3(0.0f, 1.0f, 0.0f);


	if (Senku::Input::IsKeyPressed(Senku::Key::D3))
		m_Material->mlt.baseColor = glm::vec3(0.0f, 0.0f, 1.0f);

	if (Senku::Input::IsKeyPressed(Senku::Key::D4))
	{
		if (m_Material->mlt.dissolve < 0.0f)
			m_Material->mlt.dissolve = 0.0f;
		else
			m_Material->mlt.dissolve -= 0.01f;
	}
		

	if (Senku::Input::IsKeyPressed(Senku::Key::D5))
	{
		if (m_Material->mlt.dissolve > 1.0f)
			m_Material->mlt.dissolve = 1.0f;
		else
		m_Material->mlt.dissolve += 0.01f ;
	}

	return false;
}