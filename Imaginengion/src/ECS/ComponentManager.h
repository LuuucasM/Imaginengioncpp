#pragma once

#include "ECSManager.h"
#include "ComponentArray.h"

#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <memory>
#include <iostream>

namespace IM {
	class ComponentManager {
		friend class ECSManager;
	public:
		ComponentManager(ECSManager* ecs) {
			_ECSManager = ecs;
		}
		~ComponentManager() = default;
	private:

		template<typename C_Type, typename... Args>
		C_Type& AddComponent(uint32_t entity, Args &&...args) {
			auto type_hash = typeid(C_Type).hash_code();

			if (!_ComponentArrayList.contains(type_hash)) {
				_ComponentArrayList[type_hash] = CreateRefPtr<ComponentArray<C_Type>>();
			}

			_EntityToComponentCodes[entity].insert(type_hash);

			return std::static_pointer_cast<ComponentArray<C_Type>>(_ComponentArrayList[type_hash])->AddComponent(entity, std::forward<Args>(args)...);
		}

		template<typename C_Type>
		void RemoveComponent(uint32_t entity) {
			_ComponentArrayList[typeid(C_Type).hash_code()]->RemoveComponent(entity);
			_EntityToComponentCodes[entity].erase(typeid(C_Type).hash_code());
		}

		template<typename C_Type>
		bool HasComponent(uint32_t entity) {
			auto type_hash = typeid(C_Type).hash_code();
			return _EntityToComponentCodes[entity].contains(type_hash);
		}

		template<typename C_Type>
		C_Type& GetComponent(uint32_t entity) {
			auto type_hash = typeid(C_Type).hash_code();
			return std::static_pointer_cast<ComponentArray<C_Type>>(_ComponentArrayList[type_hash])->GetComponent(entity);
		}

		template<typename ...Args>
		auto GetComponents(uint32_t entity) {
			return std::tie(GetComponent<Args>(entity)...);
		}

		template<typename ...Args>
		std::vector<uint32_t> GetGroup() {
			auto argSize = sizeof...(Args);
			if (argSize == 1) {
				return (_ComponentArrayList[typeid(Args).hash_code()]->_EntityList, ...);
			}

			std::vector<RefPtr<IComponentArray>> componentArrays;
			(componentArrays.emplace_back(_ComponentArrayList[typeid(Args).hash_code()]), ...);

			std::sort(componentArrays.begin(), componentArrays.end(), [](const RefPtr<IComponentArray>& a, const RefPtr<IComponentArray>& b) { return a->_EntityList.size() < b->_EntityList.size(); });

			std::unordered_set<uint32_t> keys(componentArrays[0]->_EntityList.begin(), componentArrays[0]->_EntityList.end());

			std::vector<uint32_t> result;
			for (uint32_t i = 1; i < componentArrays.size(); ++i) {
				for (auto entity : componentArrays[i]->_EntityList) {
					if (keys.contains(entity)) {
						result.push_back(entity);
						keys.erase(entity);
					}
				}
			}

			return result;
		}

		void DestroyEntity(uint32_t entity) {
			for (auto ele : _EntityToComponentCodes[entity]) {
				_ComponentArrayList[ele]->RemoveComponent(entity);
			}
			_EntityToComponentCodes.erase(entity);
		}
	private:
		std::unordered_map<size_t, RefPtr<IComponentArray>> _ComponentArrayList{};
		std::unordered_map<uint32_t, std::unordered_set<size_t>> _EntityToComponentCodes{};
		ECSManager*_ECSManager;
	};
}