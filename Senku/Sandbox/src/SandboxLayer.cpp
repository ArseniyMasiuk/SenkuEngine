#include "PrecompiledHeader.h"
#include "SandboxLayer.h"
#include "SenkuEngine\Renderer\Renderer.h"

#include "ImGui\imgui.h"
#include "ImGui\imgui_internal.h"

SandBoxLayer::SandBoxLayer()
	:Layer("SandBoxLayer")
{

	float width = static_cast<float>(Senku::Application::Get()->GetWindow().GetWidth());
	float height = static_cast<float>(Senku::Application::Get()->GetWindow().GetHeight());
	m_Camera = Senku::CreateRef<Senku::PerspectiveCamera>(45.0f, width / height, 0.01f, 10000.0f);

	m_Camera->SetPosition(glm::vec3(0, 0, -5));


	Senku::ModelLoader loader;
	//loader.LoadModel("Sandbox/assets/meshes/cube/untitled.obj");
	loader.LoadModel("Sandbox/assets/meshes/Cerberus_by_Andrew_Maximov/Cerberus_LP.FBX");

	Senku::Model model = loader.GetModels()[0];

	Senku::Ref<Senku::VertexArray> vertexArray = Senku::VertexArray::Create();

	Senku::Ref<Senku::VertexBuffer> vertexBuffer;
	vertexBuffer = Senku::VertexBuffer::Create((float*)(model.vertecies.data()), model.vertecies.size() * sizeof(Senku::Vertex));

	vertexBuffer->Bind();
	{
		Senku::BufferLayout layout =
		{
			{Senku::ShaderDataType::Float3, "aPos"},
			{Senku::ShaderDataType::Float3, "aNormal"},
			{Senku::ShaderDataType::Float2, "aTexCoord"}
		};
		vertexBuffer->SetLayout(layout);
	}
	vertexArray->AddVertexBuffer(vertexBuffer);

	Senku::Ref<Senku::IndexBuffer> indexBuffer;
	indexBuffer = Senku::IndexBuffer::Create(model.indices.data(), model.indices.size());


	vertexArray->SetIndexBuffer(indexBuffer);
	m_VertexArrays.push_back(std::make_pair(Transform(), vertexArray));
	m_VertexArrays.push_back(std::make_pair(Transform(), vertexArray));
	m_VertexArrays.push_back(std::make_pair(Transform(),vertexArray));
	m_Names.push_back("Cerberus.fbx");
	m_Names.push_back("Cerberus1.fbx");
	m_Names.push_back("Cerberus2.fbx");

	m_Shader = Senku::Shader::Create("Sandbox/assets/shaders/basicShader.glsl");

	//texture = Senku::Texture2D::Create("Sandbox/assets/textures/default.jpg");
	texture = Senku::Texture2D::Create("Sandbox/assets/meshes/Cerberus_by_Andrew_Maximov/Textures/Cerberus_A.tga");


	m_Material = Senku::CreateRef<Senku::MaterialInstance>(m_Shader);
	m_Material->AddTexture(texture, Senku::MaterialInstance::TextureType::eAlbedo);
	m_Material->mlt = model.mlt;

	Senku::FrameBufferSpecification spec;
	spec.Width = width;
	spec.Height = height;

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
	m_Camera->UpdateCameraPosition(timeStep);

	m_FrameBuffer->Bind();

	Senku::RenderCommand::ClearColor({ 0.1f, 0.2f, 0.2f, 0.0f });
	Senku::RenderCommand::Clear();

	Senku::Renderer::BeginScene(m_Camera); // todo:: set up cameras light shaders all stuff that should be common

	

	//glm::mat4 model = glm::scale(glm::mat4(1), glm::vec3(0.1, 0.1, 0.1)) * glm::rotate(glm::mat4(1.0f), glm::radians(-90.f), glm::vec3(1, 0, 0));

	for (auto it = m_VertexArrays.begin(); it != m_VertexArrays.end(); it++)
	{
		Senku::Renderer::Submit(m_Material, it->second, it->first.GetMatrix());
	}


	Senku::Renderer::EndScene();

	m_FrameBuffer->Unbind();
}

bool SandBoxLayer::OnEvent(Senku::Event & event)
{

	if (Senku::Input::IsKeyPressed(Senku::Key::D1))
		m_Material->mlt.baseColor = glm::vec3(1.0f, 0.0f, 0.0f);


	if (Senku::Input::IsKeyPressed(Senku::Key::D2))
		m_Material->mlt.baseColor = glm::vec3(0.0f, 1.0f, 0.0f);


	if (Senku::Input::IsKeyPressed(Senku::Key::D3))
		m_Material->mlt.baseColor = glm::vec3(0.0f, 0.0f, 1.0f);

	if (Senku::Input::IsKeyPressed(Senku::Key::D4))
	{
		if (m_Material->mlt.dissolve < 0.0f)
			m_Material->mlt.dissolve = 0.0f;
		else
			m_Material->mlt.dissolve -= 0.01f;
	}
		

	if (Senku::Input::IsKeyPressed(Senku::Key::D5))
	{
		if (m_Material->mlt.dissolve > 1.0f)
			m_Material->mlt.dissolve = 1.0f;
		else
		m_Material->mlt.dissolve += 0.01f ;
	}

	return false;
}

static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f);


void SandBoxLayer::OnImGuiRender()
{
	// If you strip some features of, this demo is pretty much equivalent to calling DockSpaceOverViewport()!
// In most cases you should be able to just call DockSpaceOverViewport() and ignore all the code below!
// In this specific demo, we are not using DockSpaceOverViewport() because:
// - we allow the host window to be floating/moveable instead of filling the viewport (when opt_fullscreen == false)
// - we allow the host window to have padding (when opt_padding == true)
// - we have a local menu bar in the host window (vs. you could use BeginMainMenuBar() + DockSpaceOverViewport() in your code!)
// TL;DR; this demo is more complicated than what you would normally use.
// If we removed all the options we are showcasing, this demo would become:
//     void ShowExampleAppDockSpace()
//     {
//         ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
//     }

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

	float width = static_cast<float>(Senku::Application::Get()->GetWindow().GetWidth());
	float height = static_cast<float>(Senku::Application::Get()->GetWindow().GetHeight());

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("Viewport");
	ImGui::PopStyleVar();

	ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
	if (m_ViewportSize != *((glm::vec2*)(&viewportPanelSize)))
	{
		m_FrameBuffer->Resize(viewportPanelSize.x, viewportPanelSize.y);
		m_Camera->Resize(viewportPanelSize.x, viewportPanelSize.y);
		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
	}

	unsigned long long id = m_FrameBuffer->GetColorAttachment();
	ImGui::Image(reinterpret_cast<void*>(id), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
	ImGui::End();

	

	if(m_ShowDemo)
		ImGui::ShowDemoWindow(&m_ShowDemo);


	ShowMeshesTree();

	ImGui::End();
}

void SandBoxLayer::ShowMeshesTree()
{
	ImGui::Begin("Meshes");
	static int item_current_idx = 0;

	ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
	
	if (ImGui::BeginListBox("##listbox", ImVec2(-FLT_MIN, viewportPanelSize.y)))
	{
		for (int n = 0; n < m_Names.size(); n++)
		{
			const bool is_selected = (item_current_idx == n);
			if (ImGui::Selectable(m_Names[n].c_str(), is_selected))
				item_current_idx = n;

			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndListBox();
	}
	ImGui::End();

	ShowMeshProperties(item_current_idx);
}

void SandBoxLayer::ShowMeshProperties(unsigned int index)
{
	ImGui::Begin("Properties");

	Transform& tr = m_VertexArrays[index].first;

	DrawVec3Control("Translation", tr.m_Translation);
	glm::vec3 rotation = glm::degrees(tr.m_Rotation);
	DrawVec3Control("Rotation", rotation);
	tr.m_Rotation = glm::radians(rotation);
	DrawVec3Control("Scale", tr.m_Scale, 1.0f);

	ImGui::End();
}


static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue /*= 0.0f*/, float columnWidth /*= 100.0f*/)
{
	ImGuiIO& io = ImGui::GetIO();
	auto boldFont = io.Fonts->Fonts[0];

	ImGui::PushID(label.c_str());

	ImGui::Columns(2);
	ImGui::SetColumnWidth(0, columnWidth);
	ImGui::Text(label.c_str());
	ImGui::NextColumn();

	ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

	float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
	ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
	ImGui::PushFont(boldFont);
	if (ImGui::Button("X", buttonSize))
		values.x = resetValue;
	ImGui::PopFont();
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
	ImGui::PushFont(boldFont);
	if (ImGui::Button("Y", buttonSize))
		values.y = resetValue;
	ImGui::PopFont();
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
	ImGui::PushFont(boldFont);
	if (ImGui::Button("Z", buttonSize))
		values.z = resetValue;
	ImGui::PopFont();
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();

	ImGui::PopStyleVar();

	ImGui::Columns(1);

	ImGui::PopID();
}