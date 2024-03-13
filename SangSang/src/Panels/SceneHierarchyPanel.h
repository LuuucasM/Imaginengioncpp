#pragma once

#include "Scene/Scene.h"
#include "ECS/Entity.h"
#include "Imgui/imgui.h"

namespace IM {
	class SceneHierarchyPanel{
	friend class PropertiesPanel;
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const WeakPtr<Scene>& scene);

		void SetContext(const WeakPtr<Scene>& scene);

		void OnImGuiRender();

		void SetSelectedEntity(Entity& entity);
		Entity GetSelectedEntity() { return _SelectionContext; }
	private:
		void DrawEntityNode(Entity entity);
	private:
		std::unordered_set<uint32_t> entityToDestroy;
		WeakPtr<Scene> _Context;
		Entity _SelectionContext{0, nullptr};
	};
}
