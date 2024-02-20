#pragma once

#include "ECSManager.h"
#include "Systems.h"

namespace IM {
	class SystemManager {
	friend class ECSManager;
	public:
		SystemManager() = default;

		~SystemManager() = default;
	private:
		template <typename T_Sys, typename... Args>
		void RegisterSystem() {
			std::set<size_t> typeHashes = { typeid(Args).hash_code()... };
			//check if this combination of component types is already cached
			auto Pairing = [](size_t a, size_t b) {
				return (int)(a >= b ? a * a + a + b : a + b * b);
			};
			size_t num = std::accumulate(typeHashes.begin(), typeHashes.end(), 0, Pairing);

			_SystemList[typeid(T_Sys).hash_code()] = CreateRefPtr<T_Sys>(num);

			_SystemList[typeid(T_Sys).hash_code()]->_TypeHashes = typeHashes;
		}

		template<typename T_Sys>
		void SystemOnUpdate(ScopePtr<ComponentManager>& _ComponentManager, float dt) {
			_SystemList[typeid(T_Sys).hash_code()]->OnUpdate(_ComponentManager, dt);
		}
	private:
		std::unordered_map<size_t, RefPtr<System>> _SystemList{};
	};
}