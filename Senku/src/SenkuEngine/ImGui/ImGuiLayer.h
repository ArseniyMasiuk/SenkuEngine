#pragma once
#include "SenkuEngine\Core\Layer.h"

namespace Senku
{
	class ImGuiLayer : Layer
	{
	public:

		ImGuiLayer(const std::string& name = "ImGuiLayer");
		virtual ~ImGuiLayer();

		void Begin(); // will set up all for rendering imgui
		void End(); // will submit all data and draw it

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(/*Timestep ts*/float timeStep) override;
		//virtual void OnImGuiRender() override;
		virtual bool OnEvent(Event& event) override;

	};
}