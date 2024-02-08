#include "SceneHierarchyPanel.h"
#include "Imagine.h"

namespace IM{
	SceneHierarchyPanel::SceneHierarchyPanel(const WeakPtr<Scene>& scene)
	{
		SetContext(scene);
	}
	void SceneHierarchyPanel::SetContext(const WeakPtr<Scene>& scene)
	{
		_Context = scene;
	}
	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene");
		if (!_Context.expired()){
			//iterate through all the entities in the ECS
			for (auto entityID : _Context.lock()->_ECSManager.GetAllEntityIDs()) {
				Entity entity{ entityID, _Context.lock().get()};
				DrawEntityNode(entity);
			}

			if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered()) {
				_SelectionContext = { 0, nullptr };
			}
		}
		ImGui::End();
	}
	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& nameComponent = entity.GetComponent<C_Name>();;
		ImGuiTreeNodeFlags flags = ((_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0)
			| ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)entity, flags, nameComponent.Name.c_str());
		if (ImGui::IsItemClicked()) {
			_SelectionContext = entity;
		}
		if (opened) {
			ImGui::TreePop();
		}
	}
}