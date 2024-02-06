#pragma once

#include "Scene/Scene.h"
#include "ECSManager.h"

#include <iostream>

namespace IM {
	class Entity {
	public:
		Entity() = default;
		Entity(uint32_t entityID, Scene *scene)
			: _EntityID(entityID), _Scene(scene){
			std::cout << "my entity ID is: " << entityID << std::endl;
		}

		Entity(const Entity& other) = default;

		template<typename C_Type, typename... Args>
		C_Type& AddComponent(Args&&... args) { 
			return _Scene->_ECSManager.AddComponent<C_Type>(_EntityID, std::forward<Args>(args)...); 
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

		operator bool() const { return _EntityID != 0; }
	private:
		uint32_t _EntityID{ 0 };
		Scene* _Scene = nullptr;
	};
}