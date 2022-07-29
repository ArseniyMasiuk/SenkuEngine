#pragma once
#include "SenkuEngine.h"




class SandBoxLayer : public Senku::Layer
{
	Senku::Ref<Senku::VertexArray> m_VertexArray;
	
	Senku::Ref<Senku::Shader> m_Shader;
	Senku::Ref<Senku::PerspectiveCamera> m_Camera;

	std::vector<glm::mat4> transform;

	Senku::Ref<Senku::Texture2D> texture;


	Senku::Ref<Senku::MaterialInstance> m_Material;

public:
	SandBoxLayer();

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(/*Timestep ts*/float timeStep) override;
	
	virtual bool OnEvent(Senku::Event& event) override;
};