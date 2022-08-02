#pragma once

#include "SenkuEngine\Renderer\Renderer.h"
#include "SenkuEngine\Renderer\Camera.h"
#include "SenkuEngine\Renderer\Shader.h"
#include "SenkuEngine\Renderer\Buffer.h"
#include "SenkuEngine\Renderer\VertexArray.h"

#include "glm\ext\quaternion_transform.hpp"
#include "glm\ext\matrix_transform.hpp"
#include "glm\gtx\quaternion.hpp"

namespace Senku
{

	struct Transform
	{
		glm::vec3 m_Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_Scale = { 1.0f, 1.0f, 1.0f };

		glm::mat4 GetMatrix()
		{
			glm::mat4 rotation = glm::toMat4(glm::quat(m_Rotation));

			return glm::translate(glm::mat4(1.0f), m_Translation)
				* rotation
				* glm::scale(glm::mat4(1.0f), m_Scale);
		}
	};

	struct Entity
	{
		std::string name;
		Transform transform;
		Ref<VertexArray> mesh;
		Ref<MaterialInstance> material;
	};

	class Scene
	{
	public:
		Scene(/*std::string path to file where is serialized scene from editor*/);

		void AddMesh(const std::string& path);

		void AddTextureToMaterial(uint32_t indexOfEntity, const std::string & path, MaterialInstance::TextureType type);
		

		// jsut to navigate through entities if necessary
		std::vector<Entity>::const_iterator cbegin() { return m_Entities.cbegin(); }
		std::vector<Entity>::const_iterator cend() { return m_Entities.cend(); }

		void Render(float timeStep);

		void Resize(uint32_t width, uint32_t height);

		// this should not be here, JUST FOR TESTING!!!
		void RenderImGui();

		bool OnEvent(Event& event);

	private:

		void ShowMeshProperties(unsigned int index);
		void ShowMeshesTree();
		void ShotMaterialProperties(unsigned int index);

		std::vector<Entity> m_Entities;

		Ref<Shader> m_Shader; // for now only one shader
		Ref<PerspectiveCamera> m_Camera;

	};
}