#include "PrecompiledHeader.h"
#include "SandboxLayer.h"
#include "SenkuEngine\Renderer\Renderer.h"

#include "ImGui\imgui.h"
#include "ImGui\imgui_internal.h"

#include "SenkuEngine\Utils\SystemUtils.h"

SandBoxLayer::SandBoxLayer()
	:Layer("SandBoxLayer")
{
	Senku::FrameBufferSpecification spec;
	spec.Width = static_cast<float>(Senku::Application::Get()->GetWindow().GetWidth());;
	spec.Height = static_cast<float>(Senku::Application::Get()->GetWindow().GetHeight());

	m_FrameBuffer = Senku::FrameBuffer::Create(spec);
}

void SandBoxLayer::OnAttach()
{
}

void SandBoxLayer::OnDetach()
{
}

void SandBoxLayer::OnUpdate(float timeStep)
{

	// check resolution to change frame buffer
	Senku::FrameBufferSpecification spec = m_FrameBuffer->GetSpecification();
	if (m_ViewportSize != glm::vec2(spec.Width, spec.Height) && m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f)
	{
		m_FrameBuffer->Resize(static_cast<uint32_t>(m_ViewportSize.x), static_cast<uint32_t>(m_ViewportSize.y));
		m_Scene.Resize(static_cast<uint32_t>(m_ViewportSize.x), static_cast<uint32_t>(m_ViewportSize.y));
	}


	m_FrameBuffer->Bind();

	m_Scene.Render(timeStep);

	m_FrameBuffer->Unbind();
}

bool SandBoxLayer::OnEvent(Senku::Event & event)
{
	if (m_bViewPortFocused)
	{
		bool handled;
		handled = m_Scene.OnEvent(event);

		return handled;
	}
	else
	{
		ImGuiIO& io = ImGui::GetIO();
		bool handled = false;

		handled |= event.isInCategory(Senku::EventCategory::EventCategoryMouse) & io.WantCaptureMouse;
		handled |= event.isInCategory(Senku::EventCategory::EventCategoryKeyboard) & io.WantCaptureKeyboard;

		return handled;
	}
	

	return false;
}

void SandBoxLayer::OnImGuiRender()
{
	static bool p_open = true;
	static bool opt_fullscreen = true;
	static bool opt_padding = false;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}
	else
	{
		dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
	}

	// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
	// and handle the pass-thru hole, so we ask Begin() to not render a background.
	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
	// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
	// all active windows docked into it will lose their parent and become undocked.
	// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
	// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace Demo", &p_open, window_flags);
	ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	// Submit the DockSpace
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}


	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Options"))
		{
			// Disabling fullscreen would allow the window to be moved to the front of other windows,
			// which we can't undo at the moment without finer window depth/z control.
			ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
			ImGui::MenuItem("ShowDemo", NULL, &m_ShowDemo);
			
			ImGui::Separator();

			if (ImGui::MenuItem("Close", NULL, false, p_open != NULL))
			{
				p_open = false;
				Senku::Application::Get()->Close();
			}
			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}


	{

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Viewport");
		ImGui::PopStyleVar();

		m_bViewPortFocused = ImGui::IsWindowFocused();

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		if (m_ViewportSize != *((glm::vec2*)(&viewportPanelSize)))
			m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

		unsigned long long id = m_FrameBuffer->GetColorAttachment();
		ImGui::Image(reinterpret_cast<void*>(id), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		ImGui::End();
	}

	

	if(m_ShowDemo)
		ImGui::ShowDemoWindow(&m_ShowDemo);

	// should be moved to sandbox layer since it should be responcible for that (also later this part will be called )
	m_Scene.RenderImGui();

	ImGui::End();
}
