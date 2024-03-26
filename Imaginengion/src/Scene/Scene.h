#pragma once

#include "ECS/ECSManager.h"
#include "Renderer/EditorCamera.h"

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

		void OnUpdateRuntime(float dt);
		void OnUpdateEditor(float dt, EditorCamera& camera);

		void OnViewportResize(size_t viewportWidth, size_t viewportHeight);

		void SetName(std::string& name) { _Name = name; }

		Entity GetPrimaryCameraEntity();

		float GetFPS() { return _FPS; }
	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);
	private:
		size_t _ViewportWidth = 0, _ViewportHeight = 0;
		ECSManager _ECSManager;
		std::string _Name = "Scene Name here!";

		float _FPS = 0.0f;
	};
}