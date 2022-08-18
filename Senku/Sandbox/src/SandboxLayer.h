#pragma once
#include "SenkuEngine.h"

#include "glm\ext\quaternion_transform.hpp"
#include "glm\ext\matrix_transform.hpp"
#include "glm\gtx\quaternion.hpp"


using namespace Senku;



class SandBoxLayer : public Senku::Layer
{
	glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
	Ref<FrameBuffer> m_FrameBuffer;

	Ref<Scene> m_Scene;


	Ref<Shader> m_Shader; // for now only one shader

	//ImGui controls
	bool m_ShowDemo = false;
	bool m_bViewPortFocused = false;

private:


public:
	SandBoxLayer();

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(/*Timestep ts*/float timeStep) override;
	
	virtual bool OnEvent(Senku::Event& event) override;

	virtual void OnImGuiRender() override;
};