#pragma once
#include "entt.hpp"
#include "SenkuEngine/Core/Base.h"
#include "Scene.h"


namespace Senku
{
	class Entity
	{
		entt::entity m_EntityHandle{ entt::null };
		Scene* m_Scene = nullptr;

	public:
		Entity() = default;

		Entity(entt::entity ent, Scene* scene)
			:m_EntityHandle(ent), m_Scene(scene) {}

		// todo: investigate - should i add move assigment too ?? 
		Entity(const Entity& other)
		{
			this->m_Scene = other.m_Scene;
			this->m_EntityHandle = other.m_EntityHandle;
		}

		Entity(const Entity&& other)
		{
			this->m_Scene = other.m_Scene;
			this->m_EntityHandle = other.m_EntityHandle;
		}
		
		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			if (HasComponent<T>())
			{
				LOG_WARN("Entity already has component!");
				return m_Scene->m_Registry.get<T>(m_EntityHandle);
			}
			T& component = m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
			//m_Scene->OnComponentAdded<T>(*this, component);
			return component;
		}

		template<typename T>
		T& GetComponent()
		{
			if (!HasComponent<T>())
				LOG_WARN("Entity does not have component!");
			return m_Scene->m_Registry.get<T>(m_EntityHandle);
		}

		template<typename T>
		bool HasComponent()
		{
			return m_Scene->m_Registry.has<T>(m_EntityHandle);
		}

		template<typename T>
		void RemoveComponent()
		{
			if (!HasComponent<T>())
				LOG_WARN("Entity does not have component!");
			m_Scene->m_Registry.remove<T>(m_EntityHandle);
		}

		operator bool() const { return m_EntityHandle != entt::null; }
		operator entt::entity() const { return m_EntityHandle; }
		operator uint32_t() const { return (uint32_t)m_EntityHandle; }

	};


}