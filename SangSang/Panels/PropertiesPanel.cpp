#include "PropertiesPanel.h"
#include "Imagine.h"


namespace IM {
	PropertiesPanel::PropertiesPanel(const WeakPtr<SceneHierarchyPanel> panel)
	{
		SetContext(panel);
	}
	void PropertiesPanel::SetContext(const WeakPtr<SceneHierarchyPanel> panel)
	{
		_SceneHierarchyPanel = panel;
	}
	void PropertiesPanel::OnImGuiRender()
	{
		ImGui::Begin("Properties");
		if (!_SceneHierarchyPanel.expired() && _SceneHierarchyPanel.lock()->_SelectionContext) {
			DrawComponents(_SceneHierarchyPanel.lock()->_SelectionContext);
		}
		ImGui::End();
	}
	void PropertiesPanel::DrawComponents(Entity entity)
	{
		if (entity.HasComponent<C_Name>()) {
			auto& nameComponent = entity.GetComponent<C_Name>();


			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy(buffer, nameComponent.Name.c_str());

			if (ImGui::InputText("Name", buffer, sizeof(buffer))) {
				nameComponent.Name = std::string(buffer);
			}
		}
		if (entity.HasComponent<C_Transform>()) {

			if (ImGui::TreeNodeEx((void*)typeid(C_Transform).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform Component")) {
				auto& transformComponent = entity.GetComponent<C_Transform>();
				ImGui::DragFloat3("Position", glm::value_ptr(transformComponent.Translation), 0.05);
				ImGui::TreePop();
			}
		}
		if (entity.HasComponent<C_Camera>()) {
			if (ImGui::TreeNodeEx((void*)typeid(C_Camera).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Camera")) {
				auto& cameraComponent = entity.GetComponent<C_Camera>();

				ImGui::Checkbox("Primary Camera", &cameraComponent._bPrimary);

				std::array<const char*, 2> projectionTypeStrings = { "Perspective", "Orthographic" };
				const char* currentProjectionTypeString = projectionTypeStrings[(int)cameraComponent._ProjectionType];
				if (ImGui::BeginCombo("Projection", currentProjectionTypeString)) {
					for (int i = 0; i < projectionTypeStrings.size(); ++i) {
						bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
						if (ImGui::Selectable(projectionTypeStrings[i], isSelected)) {
							currentProjectionTypeString = projectionTypeStrings[i];
							cameraComponent._ProjectionType = (C_Camera::ProjectionType)i;
							cameraComponent.RecalculateProjection();
						}

						if (isSelected) {
							ImGui::SetItemDefaultFocus();
						}
					}
					ImGui::EndCombo();
				}

				if (cameraComponent._ProjectionType == C_Camera::ProjectionType::Perspective) {
					float perspectiveFOV = glm::degrees(cameraComponent._PerspectiveFOV);
					if (ImGui::DragFloat("FOV", &perspectiveFOV)) {
						cameraComponent._PerspectiveFOV = glm::radians(perspectiveFOV);
						cameraComponent.RecalculateProjection();
					}

					float perspectiveNear = cameraComponent._PerspectiveicNear;
					if (ImGui::DragFloat("Near", &perspectiveNear)) {
						cameraComponent._PerspectiveicNear = perspectiveNear;
						cameraComponent.RecalculateProjection();
					}

					float perspectiveFar = cameraComponent._PerspectiveFar;
					if (ImGui::DragFloat("Far", &perspectiveFar)) {
						cameraComponent._PerspectiveFar = perspectiveFar;
						cameraComponent.RecalculateProjection();
					}
				}

				if (cameraComponent._ProjectionType == C_Camera::ProjectionType::Orthographic) {
					float orthoSize = cameraComponent._OrthographicSize;
					if (ImGui::DragFloat("Size", &orthoSize)) {
						cameraComponent._OrthographicSize = orthoSize;
						cameraComponent.RecalculateProjection();
					}

					float orthoNear = cameraComponent._OrthographicNear;
					if (ImGui::DragFloat("Near", &orthoNear)) {
						cameraComponent._OrthographicNear = orthoNear;
						cameraComponent.RecalculateProjection();
					}

					float orthoFar = cameraComponent._OrthographicFar;
					if (ImGui::DragFloat("Far", &orthoFar)) {
						cameraComponent._OrthographicFar = orthoFar;
						cameraComponent.RecalculateProjection();
					}

					ImGui::Checkbox("Fixed Aspect Ratio", &cameraComponent._bFixedAspectRatio);
				}

				ImGui::TreePop();
			}
		}
	}
}