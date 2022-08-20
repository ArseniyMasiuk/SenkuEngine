#include "PrecompiledHeader.h"
#include "SceneHierarchyPannel.h"
#include "ImGui/imgui.h"
#include <imgui/imgui_internal.h>
#include "SenkuEngine/Scene/Components.h"
#include "SenkuEngine/Core/Log.h"
#include "SenkuEngine\Scene\ModelLoader.h"
#include "SenkuEngine\Utils\SystemUtils.h"

#include "glm\gtc\type_ptr.hpp"

static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f);

void LoadModel(MeshComponent& mesh, const std::string& path);
void AddTexture(MaterialComponent& material, const std::string path, MaterialInstance::TextureType type);

SceneHierarchyPanel::SceneHierarchyPanel()
{
	// create default texture
	defaultTexture = Texture2D::Create("Sandbox/assets/textures/default.jpg");
}

SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& scene)
{
	m_Context = scene;

	// create default texture
	defaultTexture = Texture2D::Create("Sandbox/assets/textures/default.jpg");
}

void SceneHierarchyPanel::OnImGuiRender()
{
	ImGui::Begin("Scene Hierarchy");

	if (m_Context)
	{
		m_Context->m_Registry.each([&](auto entityID)
			{
				Entity entity{ entityID , m_Context.get() };
				DrawEntityNode(entity);
			});

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			m_SelectionContext = {};

		// Right-click on blank space
		if (ImGui::BeginPopupContextWindow(0, 1, false))
		{
			if (ImGui::MenuItem("Create Empty Entity"))
				m_Context->CreateEntity("Empty Entity");

			ImGui::EndPopup();
		}

	}
	ImGui::End();

	ImGui::Begin("Properties");
	if (m_SelectionContext)
	{
		DrawEntityComponents(m_SelectionContext);
	}

	ImGui::End();
}

void SceneHierarchyPanel::DrawEntityNode(Entity entity)
{
	auto& tag = entity.GetComponent<TagComponent>().Tag;

	ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
	flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
	bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
	if (ImGui::IsItemClicked())
	{
		m_SelectionContext = entity;
	}

	bool entityDeleted = false;
	if (ImGui::BeginPopupContextItem())
	{
		if (ImGui::MenuItem("Delete Entity"))
			entityDeleted = true;

		ImGui::EndPopup();
	}


	// concept of child entities what what editor dont have yet
	if (opened)
	{
		//ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
		//bool opened = ImGui::TreeNodeEx((void*)9817239, flags, tag.c_str());
		//if (opened)
		//	ImGui::TreePop();
		ImGui::TreePop();
	}

	if (entityDeleted)
	{
		m_Context->DestroyEntity(entity);
		if (m_SelectionContext == entity)
			m_SelectionContext = {};
	}
}

template<typename T, typename UIFunction>
static void DrawComponent(const std::string& name, Entity entity, UIFunction uiFunction)
{
	const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
	if (entity.HasComponent<T>())
	{
		auto& component = entity.GetComponent<T>();
		ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImGui::Separator();
		bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
		ImGui::PopStyleVar(
		);
		ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
		if (ImGui::Button("+", ImVec2{ lineHeight, lineHeight }))
		{
			ImGui::OpenPopup("ComponentSettings");
		}

		bool removeComponent = false;
		if (ImGui::BeginPopup("ComponentSettings"))
		{
			if (ImGui::MenuItem("Remove component"))
				removeComponent = true;

			ImGui::EndPopup();
		}

		if (open)
		{
			uiFunction(component);
			ImGui::TreePop();
		}

		if (removeComponent)
			entity.RemoveComponent<T>();
	}
}

template<>
void SceneHierarchyPanel::DisplayAddComponentEntry<MaterialComponent>(const std::string& entryName)
{
	if (!m_SelectionContext.HasComponent<MaterialComponent>())
	{
		if (ImGui::MenuItem(entryName.c_str()))
		{
			MaterialComponent& material = m_SelectionContext.AddComponent<MaterialComponent>();
			
			// todo: this sould be refactored. I dont like how it works
			material.Material = CreateRef<MaterialInstance>(m_Context->m_Shader);
			ImGui::CloseCurrentPopup();
		}
	}
}

template<typename T>
void SceneHierarchyPanel::DisplayAddComponentEntry(const std::string& entryName)
{
	if (!m_SelectionContext.HasComponent<T>())
	{
		if (ImGui::MenuItem(entryName.c_str()))
		{
			m_SelectionContext.AddComponent<T>();
			ImGui::CloseCurrentPopup();
		}
	}
}

void SceneHierarchyPanel::DrawEntityComponents(Entity entity)
{
	if (entity.HasComponent<TagComponent>())
	{
		std::string& tag = entity.GetComponent<TagComponent>().Tag;

		char buffer[256];
		memset(buffer, 0, sizeof(buffer));
		strcpy_s(buffer, sizeof(buffer), tag.c_str());
		if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
		{
			tag = std::string(buffer);
		}
	}

	ImGui::SameLine();
	ImGui::PushItemWidth(-1);

	if (ImGui::Button("Add Component"))
		ImGui::OpenPopup("AddComponent");

	if (ImGui::BeginPopup("AddComponent"))
	{
		DisplayAddComponentEntry<TransformComponent>("Transform Component");
		DisplayAddComponentEntry<MeshComponent>("Mesh Component");
		DisplayAddComponentEntry<MaterialComponent>("Material Component");

		ImGui::EndPopup();
	}

	ImGui::PopItemWidth();

	DrawComponent<TransformComponent>("Transform", entity, [](auto& component)
		{
			DrawVec3Control("Translation", component.Translation);
			glm::vec3 rotation = glm::degrees(component.Rotation);
			DrawVec3Control("Rotation", rotation);
			component.Rotation = glm::radians(rotation);
			DrawVec3Control("Scale", component.Scale, 1.0f);
		});


	DrawComponent<MeshComponent>("Mesh", entity, [](auto& component)
		{
			//Ref<IndexBuffer> indexBuffer = component.Mesh->GetIndexBuffer();

			//ImGui::Formatted("Indecies Count: {0}", indexBuffer->GetCount());
			if (!component.Mesh.get())
				if (ImGui::Button("Load Mesh"))
				{
					std::string filePath = FileDialog::OpenFile("Meshes files (*.obj, *.FBX)\0*.obj;*.FBX\0");
					if (!filePath.empty())
						LoadModel(component, filePath);
				}


		});

	DrawComponent<MaterialComponent>("Materials", entity, [&](auto& mlt)
		{

			if (ImGui::CollapsingHeader("Albedo"))//MaterialInstance::TextureType::eAlbedo;
			{
				unsigned int id = this->defaultTexture->GetRendererID();

				Ref<Texture2D> textureAlbedo;
				if (mlt.Material->GetTexture(textureAlbedo, MaterialInstance::TextureType::eAlbedo))
				{
					id = textureAlbedo->GetRendererID();
				}
				ImGui::Image(reinterpret_cast<void*>(id), ImVec2{ 64.0f, 64.0f }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
				ImGui::SameLine();
				{
					ImGui::BeginGroup();
					ImGuiColorEditFlags flags = ImGuiColorEditFlags_NoInputs;
					ImGui::ColorEdit3("Base Color", glm::value_ptr(mlt.Material->mlt.baseColor), flags);


					if (ImGui::Button("Browse..##LoadAlbedoTexure"))
					{
						std::string filePath = FileDialog::OpenFile("Texture files (*.jpg, *.png, *.tga)\0*.tga;*.jpg;*.png\0");
						if (!filePath.empty())
						{
							Ref<Texture2D> newTexture = Texture2D::Create(filePath);
							mlt.Material->AddTexture(newTexture, MaterialInstance::TextureType::eAlbedo);

						}
					}

					ImGui::EndGroup();
				}

			}
			//MaterialInstance::TextureType::eNormal
			if (ImGui::CollapsingHeader("Normals"))
			{
				unsigned int id = this->defaultTexture->GetRendererID();

				Ref<Texture2D> textureNormal;
				if (mlt.Material->GetTexture(textureNormal, MaterialInstance::TextureType::eNormal))
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
							mlt.Material->AddTexture(newTexture, MaterialInstance::TextureType::eNormal);

						}
					}
					ImGui::EndGroup();
				}
			}
			//MaterialInstance::TextureType::eMetalness;
			if (ImGui::CollapsingHeader("Metalness"))
			{
				unsigned int id = this->defaultTexture->GetRendererID();

				Ref<Texture2D> textureNormal;
				if (mlt.Material->GetTexture(textureNormal, MaterialInstance::TextureType::eMetalness))
				{
					id = textureNormal->GetRendererID();
				}
				ImGui::Image(reinterpret_cast<void*>(id), ImVec2{ 64.0f, 64.0f }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
				ImGui::SameLine();
				{
					ImGui::BeginGroup();
					ImGui::DragFloat("Metalness##floatSliderMetalness", &mlt.Material->mlt.metallic, 0.005f, 0.0f, 1.0f, "%.3f");
					if (ImGui::Button("Browse..##LoadMetalnessTexure"))
					{
						std::string filePath = FileDialog::OpenFile("Texture files (*.jpg, *.png, *.tga)\0*.tga;*.jpg;*.png\0");
						if (!filePath.empty())
						{
							Ref<Texture2D> newTexture = Texture2D::Create(filePath);
							mlt.Material->AddTexture(newTexture, MaterialInstance::TextureType::eMetalness);

						}
					}
					ImGui::EndGroup();
				}
			}
			//MaterialInstance::TextureType::eRoughness;
			if (ImGui::CollapsingHeader("Roughness"))
			{
				unsigned int id = this->defaultTexture->GetRendererID();

				Ref<Texture2D> textureNormal;
				if (mlt.Material->GetTexture(textureNormal, MaterialInstance::TextureType::eRoughness))
				{
					id = textureNormal->GetRendererID();
				}
				ImGui::Image(reinterpret_cast<void*>(id), ImVec2{ 64.0f, 64.0f }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
				ImGui::SameLine();
				{
					ImGui::BeginGroup();
					ImGui::DragFloat("Roughness##floatSliderRoughness", &mlt.Material->mlt.roughness, 0.005f, 0.0f, 1.0f, "%.3f");
					if (ImGui::Button("Browse..##LoadRoughnessTexure"))
					{
						std::string filePath = FileDialog::OpenFile("Texture files (*.jpg, *.png, *.tga)\0*.tga;*.jpg;*.png\0");
						if (!filePath.empty())
						{
							Ref<Texture2D> newTexture = Texture2D::Create(filePath);
							mlt.Material->AddTexture(newTexture, MaterialInstance::TextureType::eRoughness);

						}
					}
					ImGui::EndGroup();
				}
			}

			// todo: add ambient occlusion texture
			if (ImGui::CollapsingHeader("Ambient Occlusion"))
			{
				ImGui::DragFloat("Ambient occlusion##floatSliderAO", &mlt.Material->mlt.ambientOclusion, 0.005f, 0.0f, 1.0f, "%.3f");
			}
		});

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


void LoadModel(MeshComponent& mesh, const std::string& path)
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


	mesh.Mesh = vertexArray;
}


void AddTexture(MaterialComponent& material, const std::string path, MaterialInstance::TextureType type)
{
	Ref<Texture2D> texure = Texture2D::Create(path);
	material.Material->AddTexture(texure, type);
}

