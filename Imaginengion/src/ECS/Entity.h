#pragma once

#include "Scene/Scene.h"
#include "Core/UUID.h"
#include "ECS/Components.h"

#include <iostream>

namespace IM {
	class Entity {
	public:
		Entity() = default;
		Entity(uint32_t entityID, Scene *scene)
			: _EntityID(entityID), _Scene(scene){}

		Entity(const Entity& other) = default;

		template<typename C_Type, typename... Args>
		C_Type& AddComponent(Args&&... args) { 
			C_Type& component = _Scene->_ECSManager.AddComponent<C_Type>(_EntityID, std::forward<Args>(args)...);
			_Scene->OnComponentAdded<C_Type>(*this, component);
			return component;
		}

		template<typename C_Type>
		C_Type& AddComponent(C_Type newComponent) {
			C_Type& component = _Scene->_ECSManager.AddComponent(_EntityID, newComponent);
			_Scene->OnComponentAdded<C_Type>(*this, component);
			return component;
		}

		template<typename C_Type>
		void RemoveComponent() {
			_Scene->_ECSManager.RemoveComponent<C_Type>(_EntityID);
		}

		template<typename C_Type>
		C_Type& GetComponent() { 
			return _Scene->_ECSManager.GetComponent<C_Type>(_EntityID); 
		}

		template<typename ...Args>
		auto GetComponents() { 
			return _Scene->_ECSManager.GetComponents<Args...>(_EntityID); 
		}

		template<typename C_Type>
		bool HasComponent() { 
			return _Scene->_ECSManager.HasComponent<C_Type>(_EntityID); 
		}

		const UUID GetUUID() { return GetComponent<C_ID>()._ID; }

		const std::string& GetName() { return GetComponent<C_Name>().Name; }

		operator bool() const { return _EntityID != 0; }
		operator uint32_t()const { return _EntityID; }
		operator uint64_t()const { return (uint64_t)_EntityID; }

		bool operator==(const Entity& other) const { return _EntityID == other._EntityID && _Scene == other._Scene; }
		bool operator!=(const Entity& other) const { return !(*this == other); }
	private:
		uint32_t _EntityID{ 0 };
		Scene* _Scene = nullptr;
	};
}