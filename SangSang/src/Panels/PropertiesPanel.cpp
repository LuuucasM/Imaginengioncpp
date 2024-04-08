#include "PropertiesPanel.h"
#include "Imagine.h"

#include "Imgui/imgui_internal.h"

#include <glm/gtc/type_ptr.hpp>

namespace IM {

	//TODO: when to have a projects object which contains the main working folder relative to the project, then change this
	static std::filesystem::path _AssetDirectory = "assets";

	PropertiesPanel::PropertiesPanel(const WeakPtr<SceneHierarchyPanel> panel)
	{
		SetContext(panel);
		_TextureIcon = Texture2D::Create("Resources/Icons/PropertiesPanel/TextureIcon.png");

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

	static bool DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float speed = 0.075f, float columnWidth = 100.0f) {

		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

		bool changed = false;
		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.478f, 0.156f, 0.156f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.717f, 0.234f, 0.234f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.597f, 0.195f, 0.195f, 1.0f });

		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize)) {
			values.x = resetValue;
			changed = true;
		}
		ImGui::PopFont();
			
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		if (ImGui::DragFloat("##X", &values.x, speed, 0.0f, 0.0f, "%.2f")) {
			changed = true;
		}
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.156f, 0.478f, 0.156f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.234f, 0.717f, 0.234f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.195f, 0.597f, 0.195f, 1.0f });

		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize)) {
			values.y = resetValue;
			changed = true;
		}
		ImGui::PopFont();
			
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		if (ImGui::DragFloat("##Y", &values.y, speed, 0.0f, 0.0f, "%.2f")) {
			changed = true;
		}
		ImGui::PopItemWidth();
		ImGui::SameLine();

		//ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.227f, 0.576f, 0.6f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.156f, 0.306f, 0.478f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.234f, 0.459f, 0.717f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.195f, 0.328f, 0.597f, 1.0f });

		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z", buttonSize)) {
			values.z = resetValue;
			changed = true;
		}
		ImGui::PopFont();
			
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

	template<typename C_Type, typename UIFunction>
	static void DrawComponent(const std::string& name, Entity entity, UIFunction function) {

		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_Framed |
			ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_FramePadding;

		if (entity.HasComponent<C_Type>()) {

			auto& component = entity.GetComponent<C_Type>();
			ImVec2 contentRegion = ImGui::GetContentRegionAvail();

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;

			ImGui::Separator();
			bool open = ImGui::TreeNodeEx((void*)typeid(C_Type).hash_code(), treeNodeFlags, name.c_str());
			ImGui::PopStyleVar();
			
			ImGui::SameLine(contentRegion.x - lineHeight * 0.5f);
			if (ImGui::Button("...", ImVec2{ lineHeight, lineHeight })) {
				ImGui::OpenPopup("componentSettings");
			}
			

			bool removeComponent = false;
			if (ImGui::BeginPopup("componentSettings")) {
				if (ImGui::MenuItem("Remove Component")) {
					removeComponent = true;
				}
				ImGui::EndPopup();
			}

			if (open) {
				function(component);
				ImGui::TreePop();
			}

			if (removeComponent) {
				entity.RemoveComponent<C_Type>();
			}
		}
	}

	void PropertiesPanel::DrawComponents(Entity entity)
	{
		if (entity.HasComponent<C_Name>()) {
			auto& nameComponent = entity.GetComponent<C_Name>();


			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy(buffer, nameComponent.Name.c_str());

			if (ImGui::InputText("##Name", buffer, sizeof(buffer))) {
				nameComponent.Name = std::string(buffer);
			}
		}

		ImGui::SameLine();
		ImGui::PushItemWidth(-1);
		if (ImGui::Button("Add Component")) {
			ImGui::OpenPopup("AddComponent");
		}
		if (ImGui::BeginPopup("AddComponent")) {
			if (!_SceneHierarchyPanel.lock()->_SelectionContext.HasComponent<C_Camera>()) {
				if (ImGui::MenuItem("Camera")) {
					_SceneHierarchyPanel.lock()->_SelectionContext.AddComponent<C_Camera>();
					ImGui::CloseCurrentPopup();
				}
			}
			if (!_SceneHierarchyPanel.lock()->_SelectionContext.HasComponent<C_SpriteRenderer>()
				&& !_SceneHierarchyPanel.lock()->_SelectionContext.HasComponent<C_CircleRenderer>()) {
				if (ImGui::MenuItem("Sprite Renderer")) {
					_SceneHierarchyPanel.lock()->_SelectionContext.AddComponent<C_SpriteRenderer>();
					ImGui::CloseCurrentPopup();
				}
				if (ImGui::MenuItem("Circle Renderer")) {
					_SceneHierarchyPanel.lock()->_SelectionContext.AddComponent<C_CircleRenderer>();
					ImGui::CloseCurrentPopup();
				}
			}
			if (!_SceneHierarchyPanel.lock()->_SelectionContext.HasComponent<C_RigidBody2D>()) {
				if (ImGui::MenuItem("Rigid Body 2D")) {
					_SceneHierarchyPanel.lock()->_SelectionContext.AddComponent<C_RigidBody2D>();
					ImGui::CloseCurrentPopup();
				}
			}
			if (!_SceneHierarchyPanel.lock()->_SelectionContext.HasComponent<C_RectCollider2D>()
				&& !_SceneHierarchyPanel.lock()->_SelectionContext.HasComponent<C_CircleCollider2D>()) {
				if (ImGui::MenuItem("Rect Collider 2D")) {
					_SceneHierarchyPanel.lock()->_SelectionContext.AddComponent<C_RectCollider2D>();
					ImGui::CloseCurrentPopup();
				}
				if (ImGui::MenuItem("Circle Collider 2D")) {
					_SceneHierarchyPanel.lock()->_SelectionContext.AddComponent<C_CircleCollider2D>();
					ImGui::CloseCurrentPopup();
				}
			}
			ImGui::EndPopup();
		}
		ImGui::PopItemWidth();

		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_Framed |
			ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_FramePadding;

		if (entity.HasComponent<C_Transform>()) {
			ImGui::Separator();
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
		DrawComponent<C_Camera>("Camera", entity,
			[](auto& component) {
				ImGui::Checkbox("Primary Camera", &component.bPrimary);

				std::array<const char*, 2> projectionTypeStrings = { "Perspective", "Orthographic" };
				const char* currentProjectionTypeString = projectionTypeStrings[(int)component.Type];
				if (ImGui::BeginCombo("Projection", currentProjectionTypeString)) {
					for (int i = 0; i < projectionTypeStrings.size(); ++i) {
						bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
						if (ImGui::Selectable(projectionTypeStrings[i], isSelected)) {
							currentProjectionTypeString = projectionTypeStrings[i];
							component.Type = (C_Camera::ProjectionType)i;
							component.RecalculateProjection();
						}

						if (isSelected) {
							ImGui::SetItemDefaultFocus();
						}
					}
					ImGui::EndCombo();
				}

				if (component.Type == C_Camera::ProjectionType::Perspective) {
					float perspectiveFOV = glm::degrees(component.PerspectiveFOV);
					if (ImGui::DragFloat("FOV", &perspectiveFOV)) {
						component.PerspectiveFOV = glm::radians(perspectiveFOV);
						component.RecalculateProjection();
					}

					float perspectiveNear = component.PerspectiveicNear;
					if (ImGui::DragFloat("Near", &perspectiveNear)) {
						component.PerspectiveicNear = perspectiveNear;
						component.RecalculateProjection();
					}

					float perspectiveFar = component.PerspectiveFar;
					if (ImGui::DragFloat("Far", &perspectiveFar)) {
						component.PerspectiveFar = perspectiveFar;
						component.RecalculateProjection();
					}
				}

				if (component.Type == C_Camera::ProjectionType::Orthographic) {
					float orthoSize = component.OrthographicSize;
					if (ImGui::DragFloat("Size", &orthoSize)) {
						component.OrthographicSize = orthoSize;
						component.RecalculateProjection();
					}

					float orthoNear = component.OrthographicNear;
					if (ImGui::DragFloat("Near", &orthoNear)) {
						component.OrthographicNear = orthoNear;
						component.RecalculateProjection();
					}

					float orthoFar = component.OrthographicFar;
					if (ImGui::DragFloat("Far", &orthoFar)) {
						component.OrthographicFar = orthoFar;
						component.RecalculateProjection();
					}

					ImGui::Checkbox("Fixed Aspect Ratio", &component.bFixedAspectRatio);
				}
			});

		DrawComponent<C_SpriteRenderer>("Sprite Renderer", entity, 
			[&](auto& component) {
				constexpr float padding = 16.0f;
				constexpr float thumbnailSize = 70.0f;

				ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));

				ImGui::ImageButton((ImTextureID)_TextureIcon->GetID(), {thumbnailSize, thumbnailSize}, {0, 1}, {1, 0});
				if (ImGui::BeginDragDropTarget()) {
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {
						const char* path = (const char*)payload->Data;
						std::filesystem::path texturePath = _AssetDirectory / path;
						component.Texture = Texture2D::Create(texturePath.string());
					}
					ImGui::EndDragDropTarget();
				}

				ImGui::DragFloat("Tiling Factor", &component.TilingFactor, 0.1f, 0.0f, 100.0f);
			});

		DrawComponent<C_CircleRenderer>("Circle Renderer", entity,
			[&](auto& component) {
				ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));
				ImGui::DragFloat("Thickness", &component.Thickness, 0.025f, 0.0f, 1.0f);
				ImGui::DragFloat("Fade", &component.Fade, 0.00025f, 0.0f, 1.0f);
			});

		DrawComponent<C_RigidBody2D>("RigidBody 2D", entity,
			[&](auto& component) {
				std::array<const char*, 3> bodyTypeStrings = { "Static", "Dynamic", "Kinematic"};
				const char* currentBodyTypeString = bodyTypeStrings[(int)component.Type];
				if (ImGui::BeginCombo("Body Type", currentBodyTypeString)) {
					for (int i = 0; i < bodyTypeStrings.size(); ++i) {
						bool isSelected = currentBodyTypeString == bodyTypeStrings[i];
						if (ImGui::Selectable(bodyTypeStrings[i], isSelected)) {
							currentBodyTypeString = bodyTypeStrings[i];
							component.Type = (C_RigidBody2D::BodyType)i;
						}

						if (isSelected) {
							ImGui::SetItemDefaultFocus();
						}
					}
					ImGui::EndCombo();
				}

				ImGui::Checkbox("Fixed Rotation", &component.bFixedRotation);
			});

		DrawComponent<C_RectCollider2D>("Collider 2D", entity,
			[&](auto& component) {
				ImGui::DragFloat2("Offset", glm::value_ptr(component.Offset));
				ImGui::DragFloat2("Size", glm::value_ptr(component.Size));
				ImGui::DragFloat("Density", &component.Density, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Friction", &component.Friction, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Restitution", &component.Restitution, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Restitution Threshold", &component.RestitutionThreshold, 0.01f, 0.0f);
			});

		DrawComponent<C_CircleCollider2D>("Collider 2D", entity,
			[&](auto& component) {
				ImGui::DragFloat2("Offset", glm::value_ptr(component.Offset));
				ImGui::DragFloat("Radius", &component.Radius);
				ImGui::DragFloat("Density", &component.Density, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Friction", &component.Friction, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Restitution", &component.Restitution, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Restitution Threshold", &component.RestitutionThreshold, 0.01f, 0.0f);
			});
	}
}