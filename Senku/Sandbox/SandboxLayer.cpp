#include "PrecompiledHeader.h"
#include "SandboxLayer.h"


#include <GL\glew.h>


SandBoxLayer::SandBoxLayer()
{
	glGenVertexArrays(1, &m_VertexArray);
	glBindVertexArray(m_VertexArray);


	float vertecies[3 * 4] =
	{
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.5f, 0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f
	};

	m_VertexBuffer = Senku::VertexBuffer::Create(vertecies, sizeof(vertecies));

	m_VertexBuffer->Bind();


	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, nullptr);



	unsigned int indecies[6] =
	{
		0, 1, 2,
		0, 2, 3
	};
	m_IndexBuffer = Senku::IndexBuffer::Create(indecies, 6);

	m_IndexBuffer->Bind();

	/*

	*/


	std::string vertexShader = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
  
out vec4 vertexPosition; 

void main()
{
    gl_Position = vec4(aPos, 1.0);
}
)";

	std::string fragmentShader = R"(
#version 330 core
out vec4 FragColor;
  
in vec4 vertexPosition;
uniform vec4 u_Color;
void main()
{
    FragColor = u_Color;
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

	if (Senku::Input::IsKeyPressed(Senku::Key::A))
	{
		m_Shader->setUniform4f("u_Color", 0.1f, 0.7f, 0.4f, 0.8f);
	}

	glBindVertexArray(m_VertexArray);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

	// shader. For now will use default shader


}

bool SandBoxLayer::OnEvent(Senku::Event & event)
{


	return false;
}
