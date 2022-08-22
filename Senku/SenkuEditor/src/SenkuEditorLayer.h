#pragma once
#include "SenkuEngine.h"

#include "glm\ext\quaternion_transform.hpp"
#include "glm\ext\matrix_transform.hpp"
#include "glm\gtx\quaternion.hpp"

#include "ScenePannel\SceneHierarchyPannel.h"

namespace Senku
{

	class SenkuEditorLayer : public Senku::Layer
	{
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
		Ref<FrameBuffer> m_FrameBuffer;

		Ref<Scene> m_Scene;

		SceneHierarchyPanel m_SceneHierarchyPanel;

		//ImGui controls
		bool m_ShowDemo = false;
		bool m_bViewPortFocused = false;

	private:


	public:
		SenkuEditorLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(/*Timestep ts*/float timeStep) override;

		virtual bool OnEvent(Senku::Event& event) override;

		virtual void OnImGuiRender() override;
	};

}
