#pragma once

#include "Scene/Scene.h"
#include "Core/Core.h"
#include "imgui/imgui.h"

namespace IM {
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const RefPtr<Scene>& scene);

		void SetContext(const RefPtr<Scene>& scene);

		void OnImGuiRender();
	private:
		RefPtr<Scene> _Context;
	};
}
