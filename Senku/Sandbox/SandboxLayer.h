#pragma once
#include "SenkuEngine.h"


#include "SenkuEngine\Renderer\Shader.h"
#include "SenkuEngine\Renderer\Buffer.h"
#include "SenkuEngine\Renderer\VertexArray.h"

#include "SenkuEngine\Renderer\Camera.h"


class SandBoxLayer : public Senku::Layer
{
	Senku::Ref<Senku::VertexArray> m_VertexArray;
	
	Senku::Ref<Senku::Shader> m_Shader;
	Senku::Ref<Senku::PerspectiveCamera> m_Camera;

	std::vector<glm::mat4> transform;

public:
	SandBoxLayer();

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(/*Timestep ts*/float timeStep) override;
	
	virtual bool OnEvent(Senku::Event& event) override;
};