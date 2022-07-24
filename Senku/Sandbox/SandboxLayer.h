#pragma once
#include "SenkuEngine.h"


#include "SenkuEngine\Renderer\Shader.h"
#include "SenkuEngine\Renderer\Buffer.h"
#include "SenkuEngine\Renderer\VertexArray.h"


class SandBoxLayer : public Senku::Layer
{
	Senku::Ref<Senku::VertexArray> m_VertexArray;
	
	Senku::Ref<Senku::Shader> m_Shader;
public:
	SandBoxLayer();

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(/*Timestep ts*/) override;
	
	virtual bool OnEvent(Senku::Event& event) override;
};