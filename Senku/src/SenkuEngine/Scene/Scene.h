#pragma once

#include "SenkuEngine\Renderer\Renderer.h"
#include "SenkuEngine\Renderer\Camera.h"
#include "SenkuEngine\Renderer\Shader.h"
#include "SenkuEngine\Renderer\Buffer.h"
#include "SenkuEngine\Renderer\VertexArray.h"
#include "SenkuEngine\Renderer\Light.h"

#include "glm\ext\quaternion_transform.hpp"
#include "glm\ext\matrix_transform.hpp"
#include "glm\gtx\quaternion.hpp"

#include "entt.hpp"


namespace Senku
{
	class Entity;

	class Scene
	{
	public:
		Scene(/*std::string path to file where is serialized scene from editor*/);

		// this should not be here, JUST FOR TESTING!!!
		void RenderImGui();
		

		void Render(float timeStep);
		void Resize(uint32_t width, uint32_t height);
		
		bool OnEvent(Event& event);

	public:
		//todo: add following
		/*
		* copy scene to another object
		*
		* add entity
		* add component to entity
		*
		* get entity
		* get component to entity
		*
		*
		*/

		Entity CreateEntity(const std::string& name);
		void DestroyEntity(Entity& entity);

	private:

		entt::registry m_Registry;

		
		
		void ShowEnvironmentPanel();


		Ref<PerspectiveCamera> m_Camera;
		DirectLight dirLight;

		Ref<Texture2D> defaultTexture;

		friend class Entity;
		friend class SceneHierarchyPanel;
	};
}