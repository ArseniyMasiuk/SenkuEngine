#pragma once
#include "Base.h"

namespace Senku
{
	class Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(/*Timestep ts*/float timeStep) {}
		//virtual void OnImGuiRender() {}
		virtual bool OnEvent(Event& event) { return false; }

		const std::string& GetName() const { return m_DebugName; }
	protected:
		std::string m_DebugName;
	};
}