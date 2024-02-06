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
		virtual bool HasComponent(uint32_t entity) = 0;

		std::unordered_map<uint32_t, size_t> _EntityToIndexMap{};
	};

	template<typename C_Type>
	class ComponentArray : public IComponentArray {
	public:
		ComponentArray() = default;
		~ComponentArray() = default;

		template<typename... Args>
		C_Type& AddComponent(uint32_t entity, Args &&...args) {
			_EntityToIndexMap[entity] = _VectorOfComponents.size();
			_VectorOfComponents.emplace_back(C_Type{args...});
			return _VectorOfComponents.back();
		}

		void RemoveComponent(uint32_t entity) override{
			std::swap(_VectorOfComponents[_EntityToIndexMap[entity]], _VectorOfComponents[_VectorOfComponents.size()-1]);
			std::swap(_EntityToIndexMap.at((uint32_t)_VectorOfComponents.size() - 1), _EntityToIndexMap.at(entity));

			_VectorOfComponents.pop_back();
			_EntityToIndexMap.erase(entity);
		}

		bool HasComponent(uint32_t entity) override {
			return (_EntityToIndexMap.find(entity) != _EntityToIndexMap.end());
		}

		auto& GetComponent(uint32_t entity) {
			return _VectorOfComponents[_EntityToIndexMap[entity]];
		}

		std::vector<C_Type>::iterator begin() { return _VectorOfComponents.begin(); }
		std::vector<C_Type>::iterator end() { return _VectorOfComponents.end(); }
		std::vector<C_Type>::iterator rbegin() { return _VectorOfComponents.rbegin(); }
		std::vector<C_Type>::iterator rend() { return _VectorOfComponents.rend(); }

		std::vector<C_Type>::iterator begin() const { return _VectorOfComponents.begin(); }
		std::vector<C_Type>::iterator end() const { return _VectorOfComponents.end(); }
		std::vector<C_Type>::iterator rbegin() const { return _VectorOfComponents.rbegin(); }
		std::vector<C_Type>::iterator rend() const { return _VectorOfComponents.rend(); }
	private:
		std::vector<C_Type> _VectorOfComponents;
	};
}