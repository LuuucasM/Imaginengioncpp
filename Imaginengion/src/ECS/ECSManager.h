#pragma once

#include "Core/Core.h"

#include "EntityManager.h"
#include "ComponentManager.h"
#include "SystemManager.h"

namespace IM {
	class ECSManager {
	public:
		ECSManager() {
			_EntityManager = CreateScopePtr<EntityManager>();
			_ComponentManager = CreateScopePtr<ComponentManager>();
			_SystemManager = CreateScopePtr<SystemManager>();
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

		std::unordered_set<uint32_t>& GetAllEntityID() {
			return _EntityManager->GetAllEntityID();
		}

		//-------------Components--------------
		template<typename C_Type>
		void RegisterComponent() {
			_ComponentManager->RegisterComponent<C_Type>();
		}
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
			return _ComponentManager->HasComponent<C_Type>(entity);
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
		const std::vector<uint32_t>& GetGroup() {
			return _ComponentManager->GetGroup<Args...>();
		}

		//Systems
		template<typename S_Type, typename... Args>
		void RegisterSystem() {
			_SystemManager->RegisterSystem<S_Type, Args...>();
		}
		template<typename S_Type>
		void SystemOnUpdate(float dt) {
			_SystemManager->SystemOnUpdate<S_Type>(_ComponentManager, dt);
		}

	private:
		ScopePtr<EntityManager> _EntityManager;
		ScopePtr<ComponentManager> _ComponentManager;
		ScopePtr<SystemManager> _SystemManager;
	};
}