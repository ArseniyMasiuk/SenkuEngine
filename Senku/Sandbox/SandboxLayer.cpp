#include "PrecompiledHeader.h"
#include "SandboxLayer.h"


#include <GL\glew.h>

namespace Senku
{
	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:    return GL_FLOAT;
		case ShaderDataType::Float2:   return GL_FLOAT;
		case ShaderDataType::Float3:   return GL_FLOAT;
		case ShaderDataType::Float4:   return GL_FLOAT;
		case ShaderDataType::Mat3:     return GL_FLOAT;
		case ShaderDataType::Mat4:     return GL_FLOAT;
		case ShaderDataType::Int:      return GL_INT;
		case ShaderDataType::Int2:     return GL_INT;
		case ShaderDataType::Int3:     return GL_INT;
		case ShaderDataType::Int4:     return GL_INT;
		case ShaderDataType::Bool:     return GL_BOOL;
		}

		ASSERT(false);
		return 0;
	}
}

SandBoxLayer::SandBoxLayer()
{
	glGenVertexArrays(1, &m_VertexArray);
	glBindVertexArray(m_VertexArray);


	float vertecies[7 * 4] =
	{
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f
	};

	m_VertexBuffer = Senku::VertexBuffer::Create(vertecies, sizeof(vertecies));

	m_VertexBuffer->Bind();


	{

		Senku::BufferLayout layout =
		{
			{Senku::ShaderDataType::Float3, "aPos"},
			{Senku::ShaderDataType::Float4, "aColor"}
		};
		m_VertexBuffer->SetLayout(layout);
	}

	uint32_t index = 0;
	const auto& layout = m_VertexBuffer->GetLayout();

	for (const auto& element : layout)
	{
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(index,
			element.GetComponentCount(),
			ShaderDataTypeToOpenGLBaseType(element.Type),
			element.Normalized ? GL_TRUE : GL_FALSE,
			layout.GetStride(),
			(const void*)element.Offset);
		index++;
	}

	unsigned int indecies[6] =
	{
		0, 1, 2,
		0, 2, 3
	};
	m_IndexBuffer = Senku::IndexBuffer::Create(indecies, 6);

	m_IndexBuffer->Bind();

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

	glBindVertexArray(m_VertexArray);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

	// shader. For now will use default shader


}

bool SandBoxLayer::OnEvent(Senku::Event & event)
{


	return false;
}
