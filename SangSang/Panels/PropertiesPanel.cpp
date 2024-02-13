#include "PropertiesPanel.h"
#include "Imagine.h"

#include "Imgui/imgui_internal.h"

#include <glm/gtc/type_ptr.hpp>

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
			if (ImGui::Button("Add Component")) {
				ImGui::OpenPopup("AddComponent");
			}
			if (ImGui::BeginPopup("AddComponent")) {
				if (ImGui::MenuItem("Camera")) {
					_SceneHierarchyPanel.lock()->_SelectionContext.AddComponent<C_Camera>();
					ImGui::CloseCurrentPopup();
				}
				if (ImGui::MenuItem("Sprite Renderer")) {
					_SceneHierarchyPanel.lock()->_SelectionContext.AddComponent<C_SpriteRenderer>();
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}
			DrawComponents(_SceneHierarchyPanel.lock()->_SelectionContext);
		}
		ImGui::End();
	}

	static bool DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float speed = 0.075f, float columnWidth = 100.0f) {

		bool changed = false;
		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });

		if (ImGui::Button("X", buttonSize)) {
			values.x = resetValue;
			changed = true;
		}
			
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		if (ImGui::DragFloat("##X", &values.x, speed, 0.0f, 0.0f, "%.2f")) {
			changed = true;
		}
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });

		if (ImGui::Button("Y", buttonSize)) {
			values.y = resetValue;
			changed = true;
		}
			
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		if (ImGui::DragFloat("##Y", &values.y, speed, 0.0f, 0.0f, "%.2f")) {
			changed = true;
		}
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });

		if (ImGui::Button("Z", buttonSize)) {
			values.z = resetValue;
			changed = true;
		}
			
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		if (ImGui::DragFloat("##Z", &values.z, speed, 0.0f, 0.0f, "%.2f")) {
			changed = true;
		}
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();

		return changed;
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

		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap;

		if (entity.HasComponent<C_Transform>()) {
			bool open = ImGui::TreeNodeEx((void*)typeid(C_Transform).hash_code(), treeNodeFlags, "Transform");

			if (open) {
				auto& transformComponent = entity.GetComponent<C_Transform>();
				DrawVec3Control("Translation", transformComponent.Translation);
				glm::vec3 rotation = glm::degrees(transformComponent.Rotation);
				if (DrawVec3Control("Rotation", rotation, 0.0f, 0.25f)) {
					transformComponent.Rotation = glm::radians(rotation);
				}
				DrawVec3Control("Scale", transformComponent.Scale, 1.0f);
				ImGui::TreePop();
			}
		}
		if (entity.HasComponent<C_Camera>()) {
			if (ImGui::TreeNodeEx((void*)typeid(C_Camera).hash_code(), treeNodeFlags, "Camera")) {
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
		if (entity.HasComponent<C_SpriteRenderer>()) {

			bool open = ImGui::TreeNodeEx((void*)typeid(C_SpriteRenderer).hash_code(), treeNodeFlags, "Sprite Renderer");

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
			ImGui::SameLine(ImGui::GetWindowWidth() - 25.0f);
			if (ImGui::Button("...", ImVec2{ 20, 20 })) {
				ImGui::OpenPopup("componentSettings");
			}
			ImGui::PopStyleVar();

			bool removeComponent = false;
			if (ImGui::BeginPopup("componentSettings")) {
				if (ImGui::MenuItem("Remove Component")) {
					removeComponent = true;
				}
				ImGui::EndPopup();
			}

			if (open) {
				auto& spriteRendererComponent = entity.GetComponent<C_SpriteRenderer>();
				ImGui::ColorEdit4("Color", glm::value_ptr(spriteRendererComponent.Color));
				ImGui::TreePop();
			}

			if (removeComponent) {
				entity.RemoveComponent<C_SpriteRenderer>();
			}
		}
	}
}