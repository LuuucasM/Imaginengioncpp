#include "SceneHierarchyPanel.h"
#include "Imagine.h"

#include <Imgui/imgui.h>

namespace IM{
	SceneHierarchyPanel::SceneHierarchyPanel(const WeakPtr<Scene>& scene)
	{
		SetContext(scene);
	}
	void SceneHierarchyPanel::SetContext(const WeakPtr<Scene>& scene)
	{
		_Context = scene;
		_SelectionContext = {0, nullptr};
	}
	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene");
		if (!_Context.expired()){
			//iterate through all the entities in the ECS
			std::unordered_set<uint32_t>& entityList = _Context.lock()->_ECSManager.GetAllEntityID();
			for (auto ent : entityList) {
				Entity entity{ ent, _Context.lock().get()};
				DrawEntityNode(entity);
			}

			if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered()) {
				_SelectionContext = { 0, nullptr };
			}
		}

		for (auto ent : entityToDestroy) {
			_Context.lock()->_ECSManager.DestroyEntity(ent);
		}
		entityToDestroy.clear();

		if (ImGui::BeginPopupContextWindow(0, 1 | ImGuiPopupFlags_NoOpenOverItems)) {
			if (ImGui::MenuItem("Create Empty Entity")) {
				_Context.lock()->CreateEntity("Empty Entity");
			}
			ImGui::EndPopup();
		}
		ImGui::End();
	}

	void SceneHierarchyPanel::SetSelectedEntity(Entity& entity)
	{
		_SelectionContext = entity;
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& nameComponent = entity.GetComponent<C_Name>();;
		ImGuiTreeNodeFlags flags = ((_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0)
			| ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)entity, flags, nameComponent.Name.c_str());
		if (ImGui::IsItemClicked()) {
			_SelectionContext = entity;
		}

		bool entityDeleted = false;
		if (ImGui::BeginPopupContextItem()) {
			if (ImGui::MenuItem("Delete Entity")) {
				entityDeleted = true;
			}
			ImGui::EndPopup();
		}

		if (opened) {
			ImGui::TreePop();
		}

		if (entityDeleted)
			entityToDestroy.insert(entity);
	}
}