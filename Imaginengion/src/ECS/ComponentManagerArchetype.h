#pragma once
#include "ArchetypeArray.h"

#include "Core/Log.h"

#include <set>
namespace IM {
	class ComponentManagerArchetype {
		friend class ECSManager;
		friend class System;
	public:
		ComponentManagerArchetype() = default;
		~ComponentManagerArchetype() = default;
	private:
		template<typename C_Type, typename... Args>
		C_Type& AddComponent(uint32_t entity, Args &&...args) {
			//figure out where in the archetype tree the entity will be living next
			//		add the new component to the entity signature
			//		check if an archetypearray exists for the new signature
			//		if an archetypearray exists:
			//			move all the components for the entity from its current archetypearray to the new one
			//			add the new component
			//			update _EntityToArchetype
			//		if an archetypearray doesnt exist:
			//			store it in other component manager
		}

		template<typename C_Type>
		void RemoveComponent(uint32_t entity) {
			//figure out where in the archetype tree the entity will be living next
			//		remove the component type from the entity signature
			//		check if an archetypearray exists for the new signature
			//		if an archetypearray exists:
			//			move all the components for the entity from its current archetypearray to the new one
			//			update _EntityToArchetype
			//		if an archetypearray doesnt exist:
			//			store it in other component manager
		}

		template<typename C_Type>
		bool HasComponent(uint32_t entity) {
			//return if entity signature has C_Type
		}

		template<typename C_Type>
		C_Type& GetComponent(uint32_t entity) {
			//check if entity has a valid archetype array
			//		if it does:
			//			call get component on archetype array
			//		if it does not:
			//			call get component on other component manager
			//
			//
			//
		}

		template<typename ...Args>
		auto GetComponents(uint32_t entity) {
			return std::tie(GetComponent<Args>(entity)...);
		}

		void DestroyEntity(uint32_t entity) {
			//check if entity is in archetypearray
			//		if it is:
			//			call DestroyEntity on archetype array
			//		if it is not:
			//			call destroyEntity on other component manager
		}

		template<typename... Args>
		void RegisterSystem() {
			//create a std::set<std::type_index> using args
			// for each element in the set:
			//		check if the current level in the tree contains the next type_index
			//iterate through the args creating new IarchetypeArray at intermeidate nodes and then on the last one creating an ArchetypeArray<Args...>
			//set archetypearray _bIsSystem to true;
			//add the signature set to _ArchetypeArrayToSignature
			//
			//
			//
			//
			//
		}
	private:
		//member variables go here
		std::unordered_map<uint32_t, RefPtr<IArchetypeArray>> _EntityToArchetype;
		std::unordered_map<uint32_t, std::set<std::type_index>> _EntityToSignature;
		std::unordered_map<RefPtr<IArchetypeArray>, std::set<std::type_index>> _ArchetypeArrayToSignature;
		std::unordered_map<std::type_index, RefPtr<IArchetypeArray>> _ArchetypeRoot;
		ScopePtr<ComponentManagerBucket> _ComponentManager = CreateScopePtr<ComponentManagerBucket>();
	};
}