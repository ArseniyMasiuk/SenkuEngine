#include "PrecompiledHeader.h"
#include "SandboxLayer.h"


#include <GL\glew.h>

SandBoxLayer::SandBoxLayer()
{


	float vertecies[7 * 4] =
	{
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		//-0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f
	};
	unsigned int indecies[6] =
	{
		0, 1, 2,
		//0, 2, 3
	};

	m_VertexArray = Senku::VertexArray::Create();

	Senku::Ref<Senku::VertexBuffer> vertexBuffer;
	vertexBuffer = Senku::VertexBuffer::Create(vertecies, sizeof(vertecies));

	vertexBuffer->Bind();
	{

		Senku::BufferLayout layout =
		{
			{Senku::ShaderDataType::Float3, "aPos"},
			{Senku::ShaderDataType::Float4, "aColor"}
		};
		vertexBuffer->SetLayout(layout);
	}
	m_VertexArray->AddVertexBuffer(vertexBuffer);
	


	Senku::Ref<Senku::IndexBuffer> indexBuffer;
	indexBuffer = Senku::IndexBuffer::Create(indecies, 3);
	
	
	m_VertexArray->SetIndexBuffer(indexBuffer);



	std::string vertexShader = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
  
out vec4 vertexPosition; 
out vec4 color;
void main()
{
    gl_Position = vec4(aPos, 1.0);
	color = aColor;
}
)";

	std::string fragmentShader = R"(
#version 330 core
out vec4 FragColor;
  
in vec4 vertexPosition;


in vec4 color;

void main()
{
    FragColor = color;
} 

		)";


	m_Shader = Senku::Shader::Create("Shader", vertexShader, fragmentShader);

	m_Shader->Bind();
}

void SandBoxLayer::OnAttach()
{
}

void SandBoxLayer::OnDetach()
{
}

void SandBoxLayer::OnUpdate()
{
	//LOG_TRACE("SandboxLayerUpdating");

	glClearColor(0.1f, 0.2f, 0.2f, 0.0f);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/*if (Senku::Input::IsKeyPressed(Senku::Key::A))
	{
		m_Shader->setUniform4f("u_Color", 0.1f, 0.7f, 0.4f, 0.8f);
	}*/

	//glBindVertexArray(m_VertexArray);
	m_VertexArray->Bind();
	glDrawElements(GL_TRIANGLES, m_VertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);

	// shader. For now will use default shader


}

bool SandBoxLayer::OnEvent(Senku::Event & event)
{


	return false;
}
