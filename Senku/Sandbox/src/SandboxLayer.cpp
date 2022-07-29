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
	m_Camera = Senku::CreateRef<Senku::PerspectiveCamera>(45.0f, width / height, 0.1f, 10000.0f);

	m_Camera->SetPosition(glm::vec3(0, 0, -5));


	Senku::ModelLoader loader;

	loader.LoadModel("C:/Users/Arseniy Masiuk/Desktop/Private_data/models/New folder/Cerberus_by_Andrew_Maximov/Cerberus_LP.FBX");

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

	texture = Senku::Texture2D::Create("Sandbox/assets/textures/default.jpg");
	//texture = Senku::Texture2D::Create("C:\\Users\\Arseniy Masiuk\\Desktop\\Private_data\\copy\\Hazel\\Hazel-master\\Sandbox\\assets\\textures\\ChernoLogo.png");


	m_Material = Senku::CreateRef<Senku::MaterialInstance>(m_Shader);
	m_Material->AddTexture(texture, Senku::MaterialInstance::TextureType::eAlbedo);



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

	glm::mat4 model = glm::scale(glm::mat4(1), glm::vec3(0.2, 0.2, 0.2)) * glm::rotate(glm::mat4(1.0f), glm::radians(-90.f), glm::vec3(1, 0, 0));

	Senku::Renderer::Submit(m_Material, m_VertexArray, model);


	Senku::Renderer::EndScene();
	/*if (Senku::Input::IsKeyPressed(Senku::Key::A))
	{
		m_Shader->setUniform4f("u_Color", 0.1f, 0.7f, 0.4f, 0.8f);
	}*/

}

bool SandBoxLayer::OnEvent(Senku::Event & event)
{


	return false;
}




bool loadShape(std::string path, std::vector<float>& positions, std::vector<uint32_t>& indexies)
{
	//load from file
	std::ifstream f;
	f.open(path, std::ios::in);
	if (!f.is_open())
	{
		std::cout << "Cant load file by path: " << path << std::endl;
		return false;
	}

	// Local cache of verts
	//std::vector<vector3d> verts;

	while (!f.eof())
	{
		char line[128];
		f.getline(line, 128);

		std::strstream s;
		s << line;

		char junk;

		if (line[0] == 'v')
		{
			float x, y, z, c1, c2;
			s >> junk >> x >> y >> z >> c1>> c2;
			positions.push_back(x);
			positions.push_back(y);
			positions.push_back(z);

			positions.push_back(c1);
			positions.push_back(c2);



		}

		if (line[0] == 'f')
		{
			int f[3];
			s >> junk >> f[0] >> f[1] >> f[2];
			indexies.push_back(f[0] - 1);
			indexies.push_back(f[1] - 1);
			indexies.push_back(f[2] - 1);
		}
	}
	return true;
}