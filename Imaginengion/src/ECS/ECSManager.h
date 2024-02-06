#pragma once

#include "Core/Core.h"

#include "EntityManager.h"
#include "ComponentManager.h"
#include "SystemManager.h"

namespace IM {
	class ECSManager {
	public:
		ECSManager() {
			_EntityManager = CreateScopePtr<EntityManager>(this);
			_ComponentManager = CreateScopePtr<ComponentManager>(this);
			_SystemManager = CreateScopePtr<SystemManager>(this);
		}
		~ECSManager() = default;

		//--------------Entity-----------------
		uint32_t CreateEntity() {
			return _EntityManager->CreateEntity();
		}
		void DestroyEntity(uint32_t entity) {
			_EntityManager->DestroyEntity(entity);
			_ComponentManager->DestroyEntity(entity);
		}

		std::unordered_set<uint32_t>& GetAllEntityIDs() {
			return _EntityManager->GetAllEntityIDs();
		}

		//-------------Components--------------
		template<typename C_Type, typename... Args>
		C_Type& AddComponent(uint32_t entity, Args &&...args) {
			return _ComponentManager->AddComponent<C_Type>(entity, std::forward<Args>(args)...);
		}
		template<typename C_Type>
		void RemoveComponent(uint32_t entity) {
			_ComponentManager->RemoveComponent<C_Type>(entity);
		}

		template <typename C_Type>
		bool HasComponent(uint32_t entity) {
			_ComponentManager->HasComponent<C_Type>(entity);
		}

		template<typename C_Type>
		C_Type& GetComponent(uint32_t entity) {
			return _ComponentManager->GetComponent<C_Type>(entity);
		}

		template<typename ...Args>
		auto GetComponents(uint32_t entity) {
			return _ComponentManager->GetComponents<Args...>(entity);
		}
		template<typename ...Args>
		std::vector<uint32_t> GetGroup() {
			return _ComponentManager->GetGroup<Args...>();
		}

		//Systems
		template<typename S_Type>
		void RegisterSystem() {

		}
		template<typename ...Args>
		void SystemOnUpdate() {
			(_SystemManager->SystemOnUpdate<Args>(), ...);
		}

	private:
		ScopePtr<EntityManager> _EntityManager;
		ScopePtr<ComponentManager> _ComponentManager;
		ScopePtr<SystemManager> _SystemManager;
	};
}