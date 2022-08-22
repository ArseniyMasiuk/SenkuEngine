#pragma once
#include "SenkuEngine/Core/Base.h"
#include "SenkuEngine/Scene/Scene.h"
#include "SenkuEngine/Scene/Entity.h"
#include "SenkuEngine/Renderer/Texture.h"

namespace Senku
{
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel();
		SceneHierarchyPanel(const Ref<Scene>& scene);

		void SetCurrentScene(const Ref<Scene>& scene) { m_Context = scene; }

		Entity GetCurrentSelectedEntity() { return m_SelectionContext; }

		void OnImGuiRender();
	private:
		void DrawEntityNode(Entity entity);
		void DrawEntityComponents(Entity entity);


		template<typename T>
		void DisplayAddComponentEntry(const std::string& entryName);

	private:

		Ref<Texture2D> defaultTexture;


		Entity m_SelectionContext;
		Ref<Scene> m_Context;
	};

}
