#pragma once

#include "ECS/ECSManager.h"

namespace IM {
	
	class Entity;

	class Scene {
	friend class SceneHierarchyPanel;
	friend class Entity;
	friend class SceneSerializer;
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = "Entity");
		void DestroyEntity(Entity entity);

		void OnUpdate(float dt);

		void OnViewportResize(size_t viewportWidth, size_t viewportHeight);

		void SetName(std::string& name) { _Name = name; }

		Entity GetPrimaryCameraEntity();
	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);
	private:
		size_t _ViewportWidth = 0, _ViewportHeight = 0;
		ECSManager _ECSManager;
		std::string _Name = "Scene Name here!";
	};
}