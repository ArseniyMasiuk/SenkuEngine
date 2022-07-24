#pragma once
#include "SenkuEngine.h"


#include "SenkuEngine\Renderer\Shader.h"
#include "SenkuEngine\Renderer\Buffer.h"



class SandBoxLayer : public Senku::Layer
{
	unsigned int m_VertexArray;
	Senku::Ref<Senku::VertexBuffer> m_VertexBuffer;
	Senku::Ref<Senku::IndexBuffer> m_IndexBuffer;
	
	Senku::Ref<Senku::Shader> m_Shader;
public:
	SandBoxLayer();

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(/*Timestep ts*/) override;
	
	virtual bool OnEvent(Senku::Event& event) override;
};