#include "PrecompiledHeader.h"
#include "Scene.h"

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

	Scene::Scene()
	{
		float width = static_cast<float>(Application::Get()->GetWindow().GetWidth());
		float height = static_cast<float>(Application::Get()->GetWindow().GetHeight());
		m_Camera = CreateRef<PerspectiveCamera>(glm::vec3(50, 50, 50), 45.0f, width / height, 0.01f, 10000.0f);

		m_Shader = Shader::Create("Sandbox/assets/shaders/basicShader.shader"); // for now hardcoded since i have only one good shader



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

	//void Scene::ShowMeshesTree()
	//{
	//	ImGui::Begin("Meshes");
	//	
	//	if (ImGui::Button("Add new mesh"))
	//	{
	//		std::string filePath = FileDialog::OpenFile("Meshes files (*.obj, *.FBX)\0*.obj;*.FBX\0");
	//		if(!filePath.empty())
	//			AddMesh(filePath);
	//	}
	//	static entt::entity item_current_idx = 0;

	//	ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

	//	if (ImGui::BeginListBox("##listbox", ImVec2(-FLT_MIN, viewportPanelSize.y)))
	//	{
	//		auto view = m_Registry.view<TagComponent>();

	//		static std::map<entt::entity, int> tempMap;
	//		static int index = 0;
	//		for (auto entityIt : view)
	//		{
	//			TagComponent& tag = view.get<TagComponent>(entityIt);
	//			auto pair = tempMap.insert(std::make_pair(entityIt, index));

	//			const bool is_selected = (item_current_idx == pair.second);

	//			if (ImGui::Selectable(tag.Tag.c_str(), is_selected))
	//				item_current_idx = pair.first;

	//			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
	//			if (is_selected)
	//				ImGui::SetItemDefaultFocus();
	//		}

	//		ImGui::EndListBox();
	//	}
	//	ImGui::End();

	//	ShowMeshProperties(item_current_idx);

	//	//show material textures
	//	ShotMaterialProperties(item_current_idx);

	//	ShowEnvironmentPanel();

	//}

	void Scene::ShowEnvironmentPanel()
	{
		ImGui::Begin("Environment");

		ImGui::ColorEdit3("Light color", (float*)&dirLight.m_LightColor);
		DrawVec3Control("Direction", dirLight.m_Direction);

		ImGui::End();
	}

	//void Scene::ShotMaterialProperties(entt::entity entity)
	//{
	//	ImGui::Begin("Material");

	//	if (m_Registry.has<MaterialComponent>(entity))
	//	{
	//		MaterialComponent& material = m_Registry.get<MaterialComponent>(entity);

	//		Ref<MaterialInstance>& mlt = material.Material;
	//		ImGui::Text("Textures");
	//		{
	//			if (ImGui::CollapsingHeader("Albedo"))//MaterialInstance::TextureType::eAlbedo;
	//			{
	//				unsigned int id = defaultTexture->GetRendererID();

	//				Ref<Texture2D> textureAlbedo;
	//				if (mlt->GetTexture(textureAlbedo, MaterialInstance::TextureType::eAlbedo))
	//				{
	//					id = textureAlbedo->GetRendererID();
	//				}
	//				ImGui::Image(reinterpret_cast<void*>(id), ImVec2{ 64.0f, 64.0f }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
	//				ImGui::SameLine();
	//				{
	//					ImGui::BeginGroup();
	//					ImGui::ColorEdit3("Base Color", glm::value_ptr(mlt->mlt.baseColor));


	//					if (ImGui::Button("Browse..##LoadAlbedoTexure"))
	//					{
	//						std::string filePath = FileDialog::OpenFile("Texture files (*.jpg, *.png, *.tga)\0*.tga;*.jpg;*.png\0");
	//						if (!filePath.empty())
	//						{
	//							Ref<Texture2D> newTexture = Texture2D::Create(filePath);
	//							mlt->AddTexture(newTexture, MaterialInstance::TextureType::eAlbedo);

	//						}
	//					}
	//					
	//					ImGui::EndGroup();
	//				}

	//			}
	//			//MaterialInstance::TextureType::eNormal
	//			if (ImGui::CollapsingHeader("Normals"))
	//			{
	//				unsigned int id = defaultTexture->GetRendererID();

	//				Ref<Texture2D> textureNormal;
	//				if (mlt->GetTexture(textureNormal, MaterialInstance::TextureType::eNormal))
	//				{
	//					id = textureNormal->GetRendererID();
	//				}
	//				ImGui::Image(reinterpret_cast<void*>(id), ImVec2{ 64.0f, 64.0f }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
	//				ImGui::SameLine();
	//				{
	//					ImGui::BeginGroup();
	//					if (ImGui::Button("Browse..##LoadNormalTexure"))
	//					{
	//						std::string filePath = FileDialog::OpenFile("Texture files (*.jpg, *.png, *.tga)\0*.tga;*.jpg;*.png\0");
	//						if (!filePath.empty())
	//						{
	//							Ref<Texture2D> newTexture = Texture2D::Create(filePath);
	//							mlt->AddTexture(newTexture, MaterialInstance::TextureType::eNormal);

	//						}
	//					}
	//					ImGui::EndGroup();
	//				}
	//			}
	//			//MaterialInstance::TextureType::eMetalness;
	//			if (ImGui::CollapsingHeader("Metalness"))
	//			{
	//				unsigned int id = defaultTexture->GetRendererID();

	//				Ref<Texture2D> textureNormal;
	//				if (mlt->GetTexture(textureNormal, MaterialInstance::TextureType::eMetalness))
	//				{
	//					id = textureNormal->GetRendererID();
	//				}
	//				ImGui::Image(reinterpret_cast<void*>(id), ImVec2{ 64.0f, 64.0f }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
	//				ImGui::SameLine();
	//				{
	//					ImGui::BeginGroup();
	//					ImGui::DragFloat("Metalness##floatSliderMetalness", &mlt->mlt.metallic, 0.005f, 0.0f, 1.0f, "%.3f");
	//					if (ImGui::Button("Browse..##LoadMetalnessTexure"))
	//					{
	//						std::string filePath = FileDialog::OpenFile("Texture files (*.jpg, *.png, *.tga)\0*.tga;*.jpg;*.png\0");
	//						if (!filePath.empty())
	//						{
	//							Ref<Texture2D> newTexture = Texture2D::Create(filePath);
	//							mlt->AddTexture(newTexture, MaterialInstance::TextureType::eMetalness);

	//						}
	//					}
	//					ImGui::EndGroup();
	//				}
	//			}
	//			//MaterialInstance::TextureType::eRoughness;
	//			if (ImGui::CollapsingHeader("Roughness"))
	//			{
	//				unsigned int id = defaultTexture->GetRendererID();

	//				Ref<Texture2D> textureNormal;
	//				if (mlt->GetTexture(textureNormal, MaterialInstance::TextureType::eRoughness))
	//				{
	//					id = textureNormal->GetRendererID();
	//				}
	//				ImGui::Image(reinterpret_cast<void*>(id), ImVec2{ 64.0f, 64.0f }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
	//				ImGui::SameLine();
	//				{
	//					ImGui::BeginGroup();
	//					ImGui::DragFloat("Roughness##floatSliderRoughness", &mlt->mlt.roughness, 0.005f, 0.0f, 1.0f, "%.3f");
	//					if (ImGui::Button("Browse..##LoadRoughnessTexure"))
	//					{
	//						std::string filePath = FileDialog::OpenFile("Texture files (*.jpg, *.png, *.tga)\0*.tga;*.jpg;*.png\0");
	//						if (!filePath.empty())
	//						{
	//							Ref<Texture2D> newTexture = Texture2D::Create(filePath);
	//							mlt->AddTexture(newTexture, MaterialInstance::TextureType::eRoughness);

	//						}
	//					}
	//					ImGui::EndGroup();
	//				}
	//			}

	//			// todo: add ambient occlusion texture
	//			if (ImGui::CollapsingHeader("Ambient Occlusion"))
	//			{
	//				ImGui::DragFloat("Ambient occlusion##floatSliderAO", &mlt->mlt.ambientOclusion, 0.005f, 0.0f, 1.0f, "%.3f");
	//			}

	//		}

	//	}
	//	ImGui::End();
	//}

	//void Scene::ShowMeshProperties(entt::entity entity)
	//{
	//	ImGui::Begin("Properties");

	//	if (m_Entities.size())
	//	{
	//		Transform& tr = m_Entities[index].transform;

	//		DrawVec3Control("Translation", tr.m_Translation);
	//		glm::vec3 rotation = glm::degrees(tr.m_Rotation);
	//		DrawVec3Control("Rotation", rotation);
	//		tr.m_Rotation = glm::radians(rotation);
	//		DrawVec3Control("Scale", tr.m_Scale, 1.0f);
	//	}
	//	ImGui::End();
	//}

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

}
