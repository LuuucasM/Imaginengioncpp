#pragma once
#include "ComponentArray.h"

#include "Core/Log.h"

#include <unordered_map>
#include <unordered_set>
#include <set>
#include <algorithm>
#include <numeric>
#include <memory>
#include <iostream>
#include <typeindex>

namespace IM {
	class ComponentManager {
		friend class ECSManager;
		friend class System;
	public:
		ComponentManager() = default;
		~ComponentManager() = default;
	private:

		template<typename C_Type, typename... Args>
		C_Type& AddComponent(uint32_t entity, Args &&...args) {
			auto type_hash = typeid(C_Type).hash_code();

			if (!_ComponentArrayList.contains(type_hash)) {
				_ComponentArrayList[type_hash] = CreateRefPtr<ComponentArray<C_Type>>();
				_ComponentArrayVersion[type_hash] = 0;
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
		const std::vector<uint32_t>& GetGroup() {
			std::set<size_t> typeHashes = { typeid(Args).hash_code()... };

			if (!GetGroupValidate(typeHashes)) {
				return _EmptyVector;
			}

			//get a list of ids for this group
			const std::vector<uint32_t>& ids = [&]()->const std::vector<uint32_t>&{
				if (typeHashes.size() == 1) {
					return _ComponentArrayList[*typeHashes.begin()]->_EntityList;
				}
				else {
					auto Pairing = [](size_t a, size_t b) {
						return (int)(a >= b ? a * a + a + b : a + b * b);
					};
					size_t num = std::accumulate(typeHashes.begin(), typeHashes.end(), 0, Pairing);

					return GetGroupIDs(typeHashes, num);
				}
			}();

			//sort components of the IDs so that they are contiguous in memory
			GetGroupSort(ids);

			return ids;
		}
		template<typename ...Args>
		const std::vector<uint32_t>& GetGroup(std::set<size_t>& typeHashes, size_t num) {
			//get a list of ids for this group
			const std::vector<uint32_t>& ids = GetGroupIDs(typeHashes, num);
			//sort components of the IDs so that they are contiguous in memory
			GetGroupSort(ids);

			return ids;
		}

		void DestroyEntity(uint32_t entity) {
			for (auto ele : _EntityToComponentCodes[entity]) {
				_ComponentArrayList[ele]->RemoveComponent(entity);
			}
			_EntityToComponentCodes.erase(entity);
		}
	private:
		void ComputeGroupIDs(const std::set<size_t>& typeHashes, size_t num) {
			std::vector<RefPtr<IComponentArray>> componentArrays;
			for (auto ele : typeHashes) {
				componentArrays.emplace_back(_ComponentArrayList[ele]);
			}

			std::sort(componentArrays.begin(), componentArrays.end(), [](const RefPtr<IComponentArray>& a, const RefPtr<IComponentArray>& b) { return a->_EntityList.size() < b->_EntityList.size(); });

			std::unordered_set<uint32_t> keys(componentArrays[0]->_EntityList.begin(), componentArrays[0]->_EntityList.end());

			std::vector<uint32_t> result;
			for (uint32_t i = 1; i < componentArrays.size(); ++i) {
				for (auto entity : componentArrays[i]->_EntityList) {
					if (size_t n = keys.erase(entity); n) {
						result.push_back(entity);
					}
				}
			}
			_EntityCache[num] = result;
		}

		const std::vector<uint32_t>& GetGroupIDs(std::set<size_t>& typeHashes, size_t num) {

			//if we only are getting 1 component type then we can just return that component type containers entity list
			if (typeHashes.size() == 1) {
				return _ComponentArrayList[*typeHashes.begin()]->_EntityList;
			}

			//check to see if we need to re-compute the cached IDs by checking the version numbers
			bool need_new = false;
			for (auto ele : typeHashes) {
				if (_ComponentArrayVersion[ele] != _ComponentArrayList[ele]->_VersionNumber) {
					_ComponentArrayVersion[ele] = _ComponentArrayList[ele]->_VersionNumber;
					need_new = true;
				}
			}
			if (need_new) {
				ComputeGroupIDs(typeHashes, num);
				return _EntityCache[num];
			}

			if (!_EntityCache.contains(num)) {
				ComputeGroupIDs(typeHashes, num);
			}

			return _EntityCache[num];

		}
		void GetGroupSort(const std::vector<uint32_t>& idList) {

		}
		bool GetGroupValidate(std::set<size_t>& typehashes) {
			bool valid = true;
			for (auto ele : typehashes) {
				if (!_ComponentArrayList.contains(ele)) {
					//IMAGINE_CORE_ERROR("Component list {} does not exist", ele);
					valid = false;
				}
			}
			return valid;
		}
	private:
		std::vector<uint32_t> _EmptyVector = {};
		std::unordered_map<size_t, RefPtr<IComponentArray>> _ComponentArrayList{};
		std::unordered_map<uint32_t, std::unordered_set<size_t>> _EntityToComponentCodes{};
		std::unordered_map<size_t, size_t> _ComponentArrayVersion{};
		std::unordered_map<size_t, std::vector<uint32_t>> _EntityCache{};
	};
}