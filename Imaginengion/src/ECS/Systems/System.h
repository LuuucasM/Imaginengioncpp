#pragma once

#include "ECS/ComponentManager.h"

#include <set>

namespace IM {
	class System {
	public:
		System() = default;
		System(size_t id)
			: _TypeID(id){

		}
		~System() = default;
		size_t _TypeID;
		std::set<size_t> _TypeHashes;

		template<typename C_Type>
		C_Type& GetComponent(ScopePtr<ComponentManager>& _ComponentManager, uint32_t entity) {
			return _ComponentManager->GetComponent<C_Type>(entity);
		}

		template<typename ...Args>
		auto GetComponents(ScopePtr<ComponentManager>& _ComponentManager, uint32_t entity) {
			return _ComponentManager->GetComponents<Args...>(entity);
		}

		template<typename... Args>
		const std::vector<uint32_t>& GetGroup(ScopePtr<ComponentManager>& _ComponentManager, std::set<size_t>& typeHashes, size_t num) {
			return _ComponentManager->GetGroup<Args...>(typeHashes, num);
		}
		virtual void OnUpdate(ScopePtr<ComponentManager>& _ComponentManager, float dt) = 0;
	};
}