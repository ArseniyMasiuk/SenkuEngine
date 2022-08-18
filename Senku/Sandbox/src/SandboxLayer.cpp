#include "PrecompiledHeader.h"
#include "SandboxLayer.h"
#include "SenkuEngine\Renderer\Renderer.h"

#include "ImGui\imgui.h"
#include "ImGui\imgui_internal.h"

#include "SenkuEngine\Utils\SystemUtils.h"

void LoadModel(Senku::Entity& enity, const std::string& path);
void AddTexture(MaterialComponent& material, const std::string path, MaterialInstance::TextureType type);

SandBoxLayer::SandBoxLayer()
	:Layer("SandBoxLayer")
{
	m_Scene.reset(new Scene());

	Senku::FrameBufferSpecification spec;
	spec.Width = static_cast<float>(Senku::Application::Get()->GetWindow().GetWidth());;
	spec.Height = static_cast<float>(Senku::Application::Get()->GetWindow().GetHeight());

	m_FrameBuffer = Senku::FrameBuffer::Create(spec);

	m_Shader = Shader::Create("Sandbox/assets/shaders/basicShader.shader"); // for now hardcoded since i have only one good shader


	Senku::Entity entity = m_Scene->CreateEntity("Gun");

	LoadModel(entity, "Sandbox/assets/meshes/Cerberus_by_Andrew_Maximov/Cerberus_LP.FBX");

	// add material and textures to it 
	{
		MaterialComponent& material = entity.AddComponent<MaterialComponent>(CreateRef<MaterialInstance>(m_Shader));

		AddTexture(material, "Sandbox/assets/meshes/Cerberus_by_Andrew_Maximov/Textures/Cerberus_A.tga", MaterialInstance::TextureType::eAlbedo);
		AddTexture(material, "Sandbox/assets/meshes/Cerberus_by_Andrew_Maximov/Textures/Cerberus_N.tga", MaterialInstance::TextureType::eNormal);
		AddTexture(material, "Sandbox/assets/meshes/Cerberus_by_Andrew_Maximov/Textures/Cerberus_R.tga", MaterialInstance::TextureType::eRoughness);
		AddTexture(material, "Sandbox/assets/meshes/Cerberus_by_Andrew_Maximov/Textures/Cerberus_M.tga", MaterialInstance::TextureType::eMetalness);
	}

	// example of using EnTT
#ifdef EXAMPLE_OF_ENTT_USAGE
	{
		entt::entity entity = m_Registry.create();

		// add component
		auto& tag = m_Registry.emplace<TagComponent>(entity, "TestName");
		auto& transform = m_Registry.emplace<TransformComponent>(entity);
		tag.Tag = "retreave tag component while emplacing";

		//remove compoent
		m_Registry.remove<TagComponent>(entity);

		// remove everyting
		m_Registry.clear();

		// retreive component and how to check if it exist
		if (m_Registry.has<TagComponent>(entity))
			TagComponent& tagViaget = m_Registry.get<TagComponent>(entity);


		// go over entities that have specific component
		auto view = m_Registry.view<TagComponent>();
		for (auto entityIt : view)
		{
			TagComponent& tC = view.get<TagComponent>(entityIt);
		}

		//to get multiple components
		auto group = m_Registry.group<TagComponent>(entt::get<TransformComponent>);
		for (auto entityIt : view)
		{
			auto [tag, transform] = group.get<TagComponent, TransformComponent>(entityIt);
			LOG_INFO("Tag {}", tag.Tag);
			tag.Tag = "new tag";

			auto [tag2, transform2] = group.get<TagComponent, TransformComponent>(entityIt);
			LOG_INFO("Tag {}", tag2.Tag);


		}

	}
#endif // EXAMPLE_OF_ENTT_USAGE

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
		m_Scene->Resize(static_cast<uint32_t>(m_ViewportSize.x), static_cast<uint32_t>(m_ViewportSize.y));
	}


	m_FrameBuffer->Bind();

	m_Scene->Render(timeStep);

	m_FrameBuffer->Unbind();
}

bool SandBoxLayer::OnEvent(Senku::Event & event)
{
	if (m_bViewPortFocused)
	{
		bool handled;
		handled = m_Scene->OnEvent(event);

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
	m_Scene->RenderImGui();

	ImGui::End();
}


void LoadModel(Senku::Entity& enity, const std::string& path)
{

	ModelLoader loader;
	loader.LoadModel(path);//"assets/meshes/Cerberus_by_Andrew_Maximov/Cerberus_LP.FBX"

	Model model = loader.GetModels()[0]; // for now pretendint that there is only one mode
	ASSERT(loader.GetModels().size());
	LOG_WARN("Meshes loaded: {}, But readint only first one!!!", loader.GetModels().size());

	Ref<VertexArray> vertexArray = VertexArray::Create();

	Ref<VertexBuffer> vertexBuffer;
	vertexBuffer = VertexBuffer::Create((float*)(model.vertecies.data()), model.vertecies.size() * sizeof(Vertex));

	vertexBuffer->Bind();
	{
		BufferLayout layout =
		{
			{ShaderDataType::Float3, "aPos"},
			{ShaderDataType::Float3, "aNormal"},
			{ShaderDataType::Float2, "aTexCoord"},
			{ShaderDataType::Float3, "aTangent"},
			{ShaderDataType::Float3, "aBitangent"}
		};
		vertexBuffer->SetLayout(layout);
	}
	vertexArray->AddVertexBuffer(vertexBuffer);

	Ref<IndexBuffer> indexBuffer;
	indexBuffer = IndexBuffer::Create(model.indices.data(), model.indices.size());

	vertexArray->SetIndexBuffer(indexBuffer);


	enity.AddComponent<MeshComponent>(vertexArray);
}

void AddTexture(MaterialComponent& material, const std::string path, MaterialInstance::TextureType type)
{
	Ref<Texture2D> texure = Texture2D::Create(path);
	material.Material->AddTexture(texure, type);
}

