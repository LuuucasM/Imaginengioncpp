#pragma once

#include "SceneHierarchyPanel.h"

namespace IM {
	class PropertiesPanel
	{
	public:
		PropertiesPanel() = default;
		~PropertiesPanel() = default;
		PropertiesPanel(const WeakPtr<SceneHierarchyPanel> panel);

		void SetContext(const WeakPtr<SceneHierarchyPanel> panel);

		void OnImGuiRender();
	private:
		void DrawComponents(Entity entity);
	private:
		WeakPtr<SceneHierarchyPanel> _SceneHierarchyPanel;
	};
}