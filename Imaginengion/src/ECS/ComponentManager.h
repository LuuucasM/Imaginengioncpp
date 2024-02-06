#pragma once

#include "ECSManager.h"
#include "ComponentArray.h"

#include <unordered_map>
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
			auto id_str = typeid(C_Type).name();
			if (_ComponentLocations.find(id_str) == _ComponentLocations.end()) {
				_ComponentLocations.insert({ id_str, _ComponentArrays.size() });
				_ComponentArrays.emplace_back(CreateRefPtr<ComponentArray<C_Type>>());
			}

			_EntityToComponentLocations[entity].push_back(_ComponentLocations[id_str]);

			return std::static_pointer_cast<ComponentArray<C_Type>>(_ComponentArrays[_ComponentLocations[id_str]])->AddComponent(entity, std::forward<Args>(args)...);
		}

		template<typename C_Type>
		void RemoveComponent(uint32_t entity) {
			auto id_str = typeid(C_Type).name();
			_ComponentArrays[_ComponentLocations[id_str]]->RemoveComponent(entity);
		}

		template<typename C_Type>
		bool HasComponent(uint32_t entity) {
			auto id_str = typeid(C_Type).name();
			_ComponentArrays[_ComponentLocations[id_str]]->HasComponent(entity);
		}

		template<typename C_Type>
		C_Type& GetComponent(uint32_t entity) {
			auto id_str = typeid(C_Type).name();
			return std::static_pointer_cast<ComponentArray<C_Type>>(_ComponentArrays[_ComponentLocations[id_str]])->GetComponent(entity);
		}

		template<typename ...Args>
		auto GetComponents(uint32_t entity) {
			return std::tie(
				(std::static_pointer_cast<ComponentArray<Args>>(_ComponentArrays[_ComponentLocations[typeid(Args).name()]])
					->GetComponent(entity)
				)...
			);
		}

		template<typename ...Args>
		std::vector<uint32_t> GetGroup() {

			std::vector<RefPtr<IComponentArray>> componentArrays;
			(componentArrays.emplace_back(_ComponentArrays[_ComponentLocations[typeid(Args).name()]]), ...);
			
			auto key_selector = [](auto pair) {return pair.first; };

			std::vector<uint32_t> keys(componentArrays[0]->_EntityToIndexMap.size());
			std::transform(componentArrays[0]->_EntityToIndexMap.begin(), componentArrays[0]->_EntityToIndexMap.end(), keys.begin(), key_selector);
			
			for (uint32_t i = 1; i < componentArrays.size(); ++i) {
				std::vector<uint32_t> keys2(componentArrays[i]->_EntityToIndexMap.size());
				std::transform(componentArrays[i]->_EntityToIndexMap.begin(), componentArrays[i]->_EntityToIndexMap.end(), keys2.begin(), key_selector);

				keys.erase(std::remove_if(keys.begin(), keys.end(), [&](auto val) { return std::find(keys2.begin(), keys2.end(), val) == keys2.end(); }), keys.end());
			}
			return keys;
		}

		void DestroyEntity(uint32_t entity) {
			for (auto ele : _EntityToComponentLocations[entity]) {
				_ComponentArrays[ele]->RemoveComponent(entity);
			}
			_EntityToComponentLocations.erase(entity);
		}
	private:
		std::unordered_map<const char*, size_t> _ComponentLocations{};
		std::vector<RefPtr<IComponentArray>> _ComponentArrays;
		std::unordered_map<uint32_t, std::vector<size_t>> _EntityToComponentLocations{};
		ECSManager*_ECSManager;
	};
}