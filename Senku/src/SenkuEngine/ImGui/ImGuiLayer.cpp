#include "PrecompiledHeader.h"
#include "ImGuiLayer.h"

namespace Senku
{
	ImGuiLayer::ImGuiLayer(const std::string & name)
	{
	}
	ImGuiLayer::~ImGuiLayer()
	{
	}
	void ImGuiLayer::Begin()
	{
	}
	void ImGuiLayer::End()
	{
	}
	void ImGuiLayer::OnAttach()
	{
	}
	void ImGuiLayer::OnDetach()
	{
	}
	void ImGuiLayer::OnUpdate(float timeStep)
	{
	}
	bool ImGuiLayer::OnEvent(Event & event)
	{
		return false;
	}
}