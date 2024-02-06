#pragma once

#include "ECSManager.h"

namespace IM {
	class SystemManager {
	friend class ECSManager;
	public:
		SystemManager(ECSManager* ecs) {
			_ECSManager = ecs;
		}

		~SystemManager() = default;
	private:
		template <typename T>
		void RegisterSystem() {

		}

		template<typename T>
		void SystemOnUpdate() {
			
		}
	private:
		ECSManager*_ECSManager;
	};
}