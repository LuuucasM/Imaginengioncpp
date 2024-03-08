#pragma once

#include "ECS/ComponentManagerBucket.h"

#include <set>

namespace IM {
	class System {
	public:
		System() = default;
		~System() = default;


		template<typename C_Type>
		C_Type& GetComponent(ScopePtr<ComponentManagerBucket>& _ComponentManager, uint32_t entity) {
			return _ComponentManager->GetComponent<C_Type>(entity);
		}

		template<typename ...Args>
		auto GetComponents(ScopePtr<ComponentManagerBucket>& _ComponentManager, uint32_t entity) {
			return _ComponentManager->GetComponents<Args...>(entity);
		}

		template<typename... Args>
		const std::vector<uint32_t>& GetGroup(ScopePtr<ComponentManagerBucket>& _ComponentManager) {
			return _ComponentManager->GetGroup<Args...>();
		}
		virtual void OnUpdate(ScopePtr<ComponentManagerBucket>& _ComponentManager, float dt) = 0;
	private:
		
	};
}