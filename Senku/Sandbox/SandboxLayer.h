#pragma once
#include "SenkuEngine.h"


#include "Platform\OpenGL\OpenGLShader.h"


class SandBoxLayer : public Senku::Layer
{
	unsigned int m_VertexArray, m_VertexBuffer, m_IndexBuffer;
	
	Senku::Ref<Senku::Shader> m_Shader;
public:
	SandBoxLayer();

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(/*Timestep ts*/) override;
	
	virtual bool OnEvent(Senku::Event& event) override;
};