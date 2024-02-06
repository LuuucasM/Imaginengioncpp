#pragma once

#include "ECSManager.h" 

#include <unordered_set>

namespace IM {

	class EntityManager {
	friend class ECSManager;
	public:
		EntityManager(ECSManager* ecs) {
			_ECSManager = ecs;
		}

		~EntityManager() = default;
	private:
		uint32_t CreateEntity() {
			return *IDsInUse.insert(_Gen.GetID()).first;
		}

		void DestroyEntity(uint32_t entity) {
			IDsRemoved.insert(entity);
			IDsInUse.erase(entity);
		}
		std::unordered_set<uint32_t>& GetAllEntityIDs() {
			return IDsInUse;
		}
	private:
		struct IDGen {
			uint32_t next_id = 1;
			uint32_t GetID() {
				return next_id++;
			}
		};
		IDGen _Gen;
		std::unordered_set<uint32_t> IDsInUse;
		std::unordered_set<uint32_t> IDsRemoved;
		ECSManager*_ECSManager;
	};
}