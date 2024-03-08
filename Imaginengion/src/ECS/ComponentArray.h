#pragma once

#include <unordered_map>
#include <vector>
#include <algorithm>
#include <initializer_list>

namespace IM {
	class IComponentArray {
	public:
		IComponentArray() = default;
		~IComponentArray() = default;
		IComponentArray(const IComponentArray& obj) = default;

		virtual void RemoveComponent(uint32_t entity) = 0;
		std::unordered_map<uint32_t, size_t> _EntityToIndexMap{};
		std::vector<uint32_t> _EntityList;
		size_t _VersionNumber;
	};

	template<typename C_Type>
	class ComponentArray : public IComponentArray {
	public:
		ComponentArray() = default;
		~ComponentArray() = default;

		template<typename... Args>
		C_Type& AddComponent(uint32_t entity, Args &&...args) {
			size_t loc = _ComponentList.size();
			_EntityToIndexMap[entity] = loc;
			//_ComponentList.emplace_back(C_Type{args...});
			_ComponentList.emplace_back(std::forward<Args>(args)...);
			_EntityList.emplace_back(entity);
			
			_VersionNumber++;

			return _ComponentList.back();
		}

		void RemoveComponent(uint32_t entity) override{
			//swap component and entityID in repsective lists
			std::swap(_ComponentList[_EntityToIndexMap[entity]], _ComponentList[_ComponentList.size() - 1]);
			std::swap(_EntityList[_EntityToIndexMap[entity]], _EntityList[_EntityList.size() - 1]);

			_EntityToIndexMap[_EntityList[_EntityToIndexMap[entity]]] = _EntityToIndexMap[entity];

			_ComponentList.pop_back();
			_EntityList.pop_back();
			_EntityToIndexMap.erase(entity);

			_VersionNumber++;
		}

		C_Type& GetComponent(uint32_t entity) {
			return _ComponentList[_EntityToIndexMap[entity]];
		}

		std::vector<C_Type>::iterator begin() { return _ComponentList.begin(); }
		std::vector<C_Type>::iterator end() { return _ComponentList.end(); }
		std::vector<C_Type>::iterator rbegin() { return _ComponentList.rbegin(); }
		std::vector<C_Type>::iterator rend() { return _ComponentList.rend(); }

		std::vector<C_Type>::iterator begin() const { return _ComponentList.begin(); }
		std::vector<C_Type>::iterator end() const { return _ComponentList.end(); }
		std::vector<C_Type>::iterator rbegin() const { return _ComponentList.rbegin(); }
		std::vector<C_Type>::iterator rend() const { return _ComponentList.rend(); }
	private:
		std::vector<C_Type> _ComponentList;
	};
}