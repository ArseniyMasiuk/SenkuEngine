#pragma once
#include "SenkuEngine.h"

class SandBoxLayer : public Senku::Layer
{
	unsigned int m_VertexArray, m_VertexBuffer, m_IndexBuffer;

public:
	SandBoxLayer();

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(/*Timestep ts*/) override;
	
	virtual bool OnEvent(Event& event) override;
};