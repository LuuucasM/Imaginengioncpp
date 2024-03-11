#pragma once

#include "ECS/ComponentManager.h"

#include <set>

namespace IM {
	class System {
	public:
		System() = default;
		~System() = default;


		template<typename C_Type>
		C_Type& GetComponent(ScopePtr<ComponentManager>& _ComponentManager, uint32_t entity) {
			return _ComponentManager->GetComponent<C_Type>(entity);
		}

		template<typename ...Args>
		auto GetComponents(ScopePtr<ComponentManager>& _ComponentManager, uint32_t entity) {
			return _ComponentManager->GetComponents<Args...>(entity);
		}

		template<typename... Args>
		const std::vector<uint32_t>& GetGroup(ScopePtr<ComponentManager>& _ComponentManager) {
			return _ComponentManager->GetGroup<Args...>();
		}
		virtual void OnUpdate(ScopePtr<ComponentManager>& _ComponentManager, float dt) = 0;
	private:
		
	};
}