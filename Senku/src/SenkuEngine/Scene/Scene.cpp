#include "PrecompiledHeader.h"
#include "Scene.h"

#include "SenkuEngine\Renderer\Renderer.h"

#include "ModelLoader.h"
#include "SenkuEngine\Core\Application.h"

#include "ImGui\imgui.h"
#include "ImGui\imgui_internal.h"
#include "SenkuEngine\Utils\SystemUtils.h"

#include "glm\gtc\type_ptr.hpp"

#include "Components.h"
#include "Entity.h"


namespace Senku
{
	static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f);
	static void DrawDragFloat(const std::string& label, float& value, float step = 0.1f, float min = 0.0f, float max = 1.0f, const char* fromat = "%.2f", float columnWidth = 100.0f);

	Scene::Scene()
	{
		float width = static_cast<float>(Application::Get()->GetWindow().GetWidth());
		float height = static_cast<float>(Application::Get()->GetWindow().GetHeight());
		m_Camera = CreateRef<PerspectiveCamera>(glm::vec3(50, 50, 50), 45.0f, width / height, 0.01f, 10000.0f);

		m_Shader = Shader::Create("SenkuEditor/assets/shaders/basicShader.shader"); // for now hardcoded since i have only one good shader

		dirLight.m_Direction = glm::vec3(100.0f, 0.0f, 0.0f);

	}

	void Scene::Render(float timeStep)
	{
		m_Camera->OnUpdate(timeStep);

		RenderCommand::ClearColor({ 0.1f, 0.2f, 0.2f, 0.0f });
		RenderCommand::Clear();

		Renderer::BeginScene(m_Camera, dirLight); // todo:: set up cameras light shaders all stuff that should be common

		auto group = m_Registry.group<TransformComponent>(entt::get<MeshComponent, MaterialComponent>);
		for (auto entityIt : group)
		{
			auto[transform, mesh, material] = group.get<TransformComponent, MeshComponent, MaterialComponent>(entityIt);
			
			Renderer::Submit(material, mesh, transform.GetTransform());
		}

		Renderer::EndScene();
	}

	void Scene::Resize(uint32_t width, uint32_t height)
	{
		m_Camera->Resize(width, height);
	}

	void Scene::RenderImGui()
	{
		ShowEnvironmentPanel();
		//ShowMeshesTree();
	}

	bool Scene::OnEvent(Event & event)
	{
		if (Input::IsKeyPressed(Key::R))
			m_Shader->ReloadShader("SenkuEditor/assets/shaders/basicShader.shader");
		// for now leave it commented since for camera resizing i have functuion
		// but later it would be good to have qeue of events and they will be handled each turn of main loop maybe?? will think a bit more later

		//if (event.GetEventType() == EventType::WindowResize)
		//{
		//	WindowResizeEvent& event = (WindowResizeEvent&)e;
		//	m_Camera->Resize(event.GetWidth(), event.GetHeight());

		//	return false; // return false sine changed view port shoudl be habdled everyhere
		//}

		if (event.isInCategory(EventCategory::EventCategoryMouse))
			m_Camera->OnEditorCameraUpdate(event);

		return false;
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity(m_Registry.create(), this);

		entity.AddComponent<TagComponent>(name.empty() ? "Entity" : name);
		entity.AddComponent<TransformComponent>();

		return entity;
	}

	void Scene::DestroyEntity(Entity& entity)
	{
		m_Registry.destroy(entity);
	}


	void Scene::ShowEnvironmentPanel()
	{
		ImGui::Begin("Environment");

		ImGui::ColorEdit3("Light color", (float*)&dirLight.m_LightColor);
		DrawVec3Control("Direction", dirLight.m_Direction, 0.0f, 150);

		DrawDragFloat("Light distance", dirLight.m_Distance, 0.1f, 0.0f, 1000.0f, "%.2f", 150);
		DrawDragFloat("Light Intensity", dirLight.m_Intensity, 0.1f, 0.0f, 20.0f, "%.2f", 150);

		ImGui::End();
	}

	static void DrawDragFloat(const std::string& label, float& value, float step /*= 0.1f*/, float min/* = 0.0f*/, float max /*= 1.0f*/, const char* fromat /*= "%.2f"*/, float columnWidth /*= 100.0f*/)
	{
		ImGui::PushID(label.c_str());

		static ImVec2 cell_padding(1.0f, 1.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, cell_padding);

		static ImGuiTableFlags flags1 = ImGuiTableFlags_BordersInnerV;
		if (ImGui::BeginTable("table1", 2, flags1))
		{
			{
				// lable
				ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, columnWidth);
				ImGui::TableNextColumn();
				ImGui::Text(label.c_str());
			}
			
			{
				// dragfloat 
				//ImGui::PopItemWidth();
				ImGui::TableNextColumn(); ImGui::DragFloat("##dragfloat", &value, step, min, max, fromat);
			}


			ImGui::EndTable();
		}
		ImGui::PopStyleVar();

		ImGui::PopID();

	}

	static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue /*= 0.0f*/, float columnWidth /*= 100.0f*/)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

		ImGui::PushID(label.c_str());

		static ImVec2 cell_padding(1.0f, 1.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, cell_padding);

		static ImGuiTableFlags flags1 = ImGuiTableFlags_BordersInnerV;
		if (ImGui::BeginTable("table1", 2, flags1))
		{
			{
				// lable
				ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, columnWidth);
				ImGui::TableNextColumn();
				ImGui::Text(label.c_str());
			}

			{
				ImGui::TableNextColumn();
				ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());

				float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
				ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 }); // ItemSpacing

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
				
				//////
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
				
				/////////////
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
				ImGui::SameLine();
				ImGui::PopStyleVar();// ItemSpacing
			}

			ImGui::EndTable();
		}
		ImGui::PopStyleVar();

		ImGui::PopID();
	}
}
