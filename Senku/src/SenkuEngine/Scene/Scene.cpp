#include "PrecompiledHeader.h"
#include "Scene.h"

#include "ModelLoader.h"
#include "SenkuEngine\Core\Application.h"

#include "ImGui\imgui.h"
#include "ImGui\imgui_internal.h"
#include "SenkuEngine\Utils\SystemUtils.h"

#include "glm\gtc\type_ptr.hpp"

namespace Senku
{
	static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f);

	Scene::Scene()
	{
		float width = static_cast<float>(Application::Get()->GetWindow().GetWidth());
		float height = static_cast<float>(Application::Get()->GetWindow().GetHeight());
		m_Camera = CreateRef<PerspectiveCamera>(glm::vec3(0, 0, -5), 45.0f, width / height, 0.01f, 10000.0f);

		m_Shader = Shader::Create("Sandbox/assets/shaders/basicShader.shader"); // for now hardcoded since i have only one good shader

		// create default texture
		defaultTexture = Texture2D::Create("Sandbox/assets/textures/default.jpg");

		dirLight.m_Direction = glm::vec3(100.0f, 0.0f, 0.0f);
		dirLight.m_Ambient = glm::vec3(0.5f, 0.5f, 0.5f);
		dirLight.m_Diffuse= glm::vec3(1.5f, 1.5f, 1.5f);
		dirLight.m_Specular = glm::vec3(1.5f, 1.5f, 1.5f);
	}

	void Scene::AddMesh(const std::string & path)
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
		
		Entity entity;
		entity.mesh = vertexArray;
		entity.material = CreateRef<MaterialInstance>(m_Shader);
		entity.material->mlt = model.mlt;

		{
			// Extract name from filepath
			auto lastSlash = path.find_last_of("/\\");
			lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
			auto lastDot = path.rfind('.');
			auto count = lastDot == std::string::npos ? path.size() - lastSlash : lastDot - lastSlash;
			entity.name = path.substr(lastSlash, count);
		}


		m_Entities.push_back(entity);
	}

	void Scene::AddTextureToMaterial(uint32_t indexOfEntity, const std::string & path, MaterialInstance::TextureType type)
	{
		ASSERT(m_Entities.size() != 0);

		Entity& model = m_Entities.at(indexOfEntity);

		Ref<Texture2D> texure = Texture2D::Create(path);

		model.material->AddTexture(texure, type);

	}

	void Scene::Render(float timeStep)
	{
		m_Camera->OnUpdate(timeStep);

		RenderCommand::ClearColor({ 0.1f, 0.2f, 0.2f, 0.0f });
		RenderCommand::Clear();

		Renderer::BeginScene(m_Camera, dirLight); // todo:: set up cameras light shaders all stuff that should be common

		for (auto it = m_Entities.begin(); it != m_Entities.end(); it++)
		{
			Renderer::Submit(it->material, it->mesh, it->transform.GetMatrix());
		}

		Renderer::EndScene();
	}
	void Scene::Resize(uint32_t width, uint32_t height)
	{
		m_Camera->Resize(width, height);
	}
	void Scene::RenderImGui()
	{
		ShowMeshesTree();
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

	void Scene::ShowMeshesTree()
	{
		ImGui::Begin("Meshes");
		
		if (ImGui::Button("Add new mesh"))
		{
			std::string filePath = FileDialog::OpenFile("Meshes files (*.obj, *.FBX)\0*.obj;*.FBX\0");
			if(!filePath.empty())
				AddMesh(filePath);
		}
		static int item_current_idx = 0;

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

		if (ImGui::BeginListBox("##listbox", ImVec2(-FLT_MIN, viewportPanelSize.y)))
		{
			for (int n = 0; n < m_Entities.size(); n++)
			{
				const bool is_selected = (item_current_idx == n);
				if (ImGui::Selectable(m_Entities[n].name.c_str(), is_selected))
					item_current_idx = n;

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndListBox();
		}
		ImGui::End();

		ShowMeshProperties(item_current_idx);

		//show material textures
		ShotMaterialProperties(item_current_idx);

		ShowEnvironmentPanel();

	}

	void Scene::ShowEnvironmentPanel()
	{
		ImGui::Begin("Environment");

		ImGui::ColorEdit3("Light color", (float*)&dirLight.m_LightColor);
		DrawVec3Control("Direction", dirLight.m_Direction);

		ImGui::End();
	}

	void Scene::ShotMaterialProperties(unsigned int index)
	{
		ImGui::Begin("Material");

		if (m_Entities.size())
		{
			Ref<MaterialInstance>& mlt = m_Entities[index].material;
			ImGui::Text("Textures");
			{
				if (ImGui::CollapsingHeader("Albedo"))//MaterialInstance::TextureType::eAlbedo;
				{
					unsigned int id = defaultTexture->GetRendererID();

					Ref<Texture2D> textureAlbedo;
					if (mlt->GetTexture(textureAlbedo, MaterialInstance::TextureType::eAlbedo))
					{
						id = textureAlbedo->GetRendererID();
					}
					ImGui::Image(reinterpret_cast<void*>(id), ImVec2{ 64.0f, 64.0f }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
					ImGui::SameLine();
					{
						ImGui::BeginGroup();
						ImGui::ColorEdit3("Base Color", glm::value_ptr(mlt->mlt.baseColor));


						if (ImGui::Button("Browse..##LoadAlbedoTexure"))
						{
							std::string filePath = FileDialog::OpenFile("Texture files (*.jpg, *.png, *.tga)\0*.tga;*.jpg;*.png\0");
							if (!filePath.empty())
							{
								Ref<Texture2D> newTexture = Texture2D::Create(filePath);
								mlt->AddTexture(newTexture, MaterialInstance::TextureType::eAlbedo);

							}
						}
						
						ImGui::EndGroup();
					}

				}
				//MaterialInstance::TextureType::eNormal
				if (ImGui::CollapsingHeader("Normals"))
				{
					unsigned int id = defaultTexture->GetRendererID();

					Ref<Texture2D> textureNormal;
					if (mlt->GetTexture(textureNormal, MaterialInstance::TextureType::eNormal))
					{
						id = textureNormal->GetRendererID();
					}
					ImGui::Image(reinterpret_cast<void*>(id), ImVec2{ 64.0f, 64.0f }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
					ImGui::SameLine();
					{
						ImGui::BeginGroup();
						if (ImGui::Button("Browse..##LoadNormalTexure"))
						{
							std::string filePath = FileDialog::OpenFile("Texture files (*.jpg, *.png, *.tga)\0*.tga;*.jpg;*.png\0");
							if (!filePath.empty())
							{
								Ref<Texture2D> newTexture = Texture2D::Create(filePath);
								mlt->AddTexture(newTexture, MaterialInstance::TextureType::eNormal);

							}
						}
						ImGui::EndGroup();
					}
				}
				//MaterialInstance::TextureType::eMetalness;
				if (ImGui::CollapsingHeader("Metalness"))
				{
					unsigned int id = defaultTexture->GetRendererID();

					Ref<Texture2D> textureNormal;
					if (mlt->GetTexture(textureNormal, MaterialInstance::TextureType::eMetalness))
					{
						id = textureNormal->GetRendererID();
					}
					ImGui::Image(reinterpret_cast<void*>(id), ImVec2{ 64.0f, 64.0f }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
					ImGui::SameLine();
					{
						ImGui::BeginGroup();
						ImGui::DragFloat("Metalness##floatSliderMetalness", &mlt->mlt.metallic, 0.005f, 0.0f, 1.0f, "%.3f");
						if (ImGui::Button("Browse..##LoadMetalnessTexure"))
						{
							std::string filePath = FileDialog::OpenFile("Texture files (*.jpg, *.png, *.tga)\0*.tga;*.jpg;*.png\0");
							if (!filePath.empty())
							{
								Ref<Texture2D> newTexture = Texture2D::Create(filePath);
								mlt->AddTexture(newTexture, MaterialInstance::TextureType::eMetalness);

							}
						}
						ImGui::EndGroup();
					}
				}
				//MaterialInstance::TextureType::eRoughness;
				if (ImGui::CollapsingHeader("Roughness"))
				{
					unsigned int id = defaultTexture->GetRendererID();

					Ref<Texture2D> textureNormal;
					if (mlt->GetTexture(textureNormal, MaterialInstance::TextureType::eRoughness))
					{
						id = textureNormal->GetRendererID();
					}
					ImGui::Image(reinterpret_cast<void*>(id), ImVec2{ 64.0f, 64.0f }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
					ImGui::SameLine();
					{
						ImGui::BeginGroup();
						ImGui::DragFloat("Roughness##floatSliderRoughness", &mlt->mlt.roughness, 0.005f, 0.0f, 1.0f, "%.3f");
						if (ImGui::Button("Browse..##LoadRoughnessTexure"))
						{
							std::string filePath = FileDialog::OpenFile("Texture files (*.jpg, *.png, *.tga)\0*.tga;*.jpg;*.png\0");
							if (!filePath.empty())
							{
								Ref<Texture2D> newTexture = Texture2D::Create(filePath);
								mlt->AddTexture(newTexture, MaterialInstance::TextureType::eRoughness);

							}
						}
						ImGui::EndGroup();
					}
				}

				// todo: add ambient occlusion texture
				if (ImGui::CollapsingHeader("Ambient Occlusion"))
				{
					ImGui::DragFloat("Ambient occlusion##floatSliderAO", &mlt->mlt.ambientOclusion, 0.005f, 0.0f, 1.0f, "%.3f");
				}

			}

		}
		ImGui::End();
	}

	void Scene::ShowMeshProperties(unsigned int index)
	{
		ImGui::Begin("Properties");

		if (m_Entities.size())
		{
			Transform& tr = m_Entities[index].transform;

			DrawVec3Control("Translation", tr.m_Translation);
			glm::vec3 rotation = glm::degrees(tr.m_Rotation);
			DrawVec3Control("Rotation", rotation);
			tr.m_Rotation = glm::radians(rotation);
			DrawVec3Control("Scale", tr.m_Scale, 1.0f);
		}
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

}
