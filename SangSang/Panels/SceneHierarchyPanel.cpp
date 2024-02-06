#include "SceneHierarchyPanel.h"

#include "ECS/Components.h"

namespace IM{
	SceneHierarchyPanel::SceneHierarchyPanel(const RefPtr<Scene>& scene)
	{
		SetContext(scene);
	}
	void SceneHierarchyPanel::SetContext(const RefPtr<Scene>& scene)
	{
		_Context = scene;
	}
	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");

		//iterate through all the entities in the ECS
		for (auto entity : _Context->_ECSManager.GetAllEntityIDs()) {
			auto& nameComponent = _Context->_ECSManager.GetComponent<C_Name>(entity);
			ImGui::Text("%s", nameComponent.Name.c_str());
		}

		ImGui::End();
	}
}