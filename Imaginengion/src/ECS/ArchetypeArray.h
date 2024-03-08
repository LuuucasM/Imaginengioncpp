#pragma once


#include <unordered_map>
#include <vector>
#include <typeindex>
#include <memory>
#include <tuple>

namespace IM {
	class IArchetypeArray {
	public:
		virtual ~IArchetypeArray() = default;

		//stuff needed for tree
		RefPtr<IArchetypeArray> _ParentArchetype;
		std::unordered_map<std::type_index, RefPtr<IArchetypeArray>> _ArcheTypeList{};

		//other stuff i might need?
		std::set<std::type_index> _Archetype;
		bool _bIsSystem = false;
	};

	template <typename... ComponentTypes>
	class ArchetypeArray : public IArchetypeArray {
	public:

		template<typename C_Type, typename... Args>
		C_Type& AddComponent(uint32_t entity, Args &&...args) {
			std::vector<C_Type> _ComponentList = std::get<std::vector<C_Type>>(_ComponentArray);

			size_t loc = _ComponentList.size();
			_EntityToIndexMap[entity] = loc;
			_ComponentList.emplace_back(C_Type{ args... });
			_EntityList.emplace_back(entity);

			return _ComponentList.back();
		}

		template<typename C_Type>
		void RemoveComponent(uint32_t entity) {
			std::vector<C_Type> _ComponentList = std::get<std::vector<C_Type>>(_ComponentArray);

			std::swap(_ComponentList[_EntityToIndexMap[entity]], _ComponentList[_ComponentList.size() - 1]);
			std::swap(_EntityList[_EntityToIndexMap[entity]], _EntityList[_EntityList.size() - 1]);

			_EntityToIndexMap[_EntityList[_EntityToIndexMap[entity]]] = _EntityToIndexMap[entity];

			_ComponentList.pop_back();
			_EntityList.pop_back();
			_EntityToIndexMap.erase(entity);
		}

		template<typename C_Type>
		C_Type& GetComponent(uint32_t entity) {
			return std::get<std::vector<C_Type>>(_ComponentArray)[_EntityToIndexMap[entity]];
		}

		template<typename C_Type>
		C_Type&& MoveComponent(uint32_t entity) {
			return std::move(std::get<std::vector<C_Type>>(_ComponentArray)[_EntityToIndexMap[entity]]);
		}

	private:
		std::tuple<std::vector<ComponentTypes>...> _ComponentArray;
		std::unordered_map<uint32_t, size_t> _EntityToIndexMap{};
		std::vector<uint32_t> _EntityList;
	};
}