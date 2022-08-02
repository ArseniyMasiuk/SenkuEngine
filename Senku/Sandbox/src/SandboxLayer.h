#pragma once
#include "SenkuEngine.h"

#include "glm\ext\quaternion_transform.hpp"
#include "glm\ext\matrix_transform.hpp"
#include "glm\gtx\quaternion.hpp"


class SandBoxLayer : public Senku::Layer
{
	glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
	Senku::Ref<Senku::FrameBuffer> m_FrameBuffer;

	Senku::Scene m_Scene;

	//ImGui controls
	bool m_ShowDemo = false;
	bool m_bViewPortFocused = false;

private:

	void ShowMeshesTree();
	void ShowMeshProperties(unsigned int index);


public:
	SandBoxLayer();

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(/*Timestep ts*/float timeStep) override;
	
	virtual bool OnEvent(Senku::Event& event) override;

	virtual void OnImGuiRender() override;
};