#pragma once

#include <string>
#include "glm\glm.hpp"
#include <glm/gtx/quaternion.hpp>

#include "SenkuEngine\Core\Base.h"
#include "SenkuEngine/Renderer/VertexArray.h"
#include "SenkuEngine/Renderer/Material.h"

namespace Senku
{
	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;//{ LOG_INFO("Created tag component"); }
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			: Tag(tag) {}

		operator const std::string() { return Tag; }
		operator std::string() { return Tag; }

		operator const std::string& () { return Tag; }
		operator std::string& () { return Tag; }

		//~TagComponent() { LOG_INFO("Destrouing TagComponent"); }
	};

	struct TransformComponent
	{
		glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

		TransformComponent() = default;//{ LOG_INFO("Created transform component"); };
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& translation)
			: Translation(translation) {}

		glm::mat4 GetTransform()
		{
			return glm::translate(glm::mat4(1.0f), Translation)
				* glm::toMat4(glm::quat(Rotation))
				* glm::scale(glm::mat4(1.0f), Scale);
		}

		//~TransformComponent() { LOG_INFO("Destrouing TransformComponent"); }

	};

	struct MeshComponent
	{
		Ref<VertexArray> Mesh;

		MeshComponent() = default;
		MeshComponent(const MeshComponent&) = default;
		MeshComponent(Ref<VertexArray> mesh)
			:Mesh(mesh)
		{}

		operator const Ref<VertexArray>&() const { return Mesh; }
		operator Ref<VertexArray>&() { return Mesh; }
	};

	struct MaterialComponent
	{
		Ref<MaterialInstance> Material;

		MaterialComponent() = default;
		MaterialComponent(const MaterialComponent&) = default;
		MaterialComponent(Ref<MaterialInstance> material)
			:Material(material)
		{}

		operator const Ref<MaterialInstance>&() const { return Material; }
		operator Ref<MaterialInstance>&() { return Material; }
	};
}
