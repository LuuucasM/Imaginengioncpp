#include "impch.h"
#include "ToolbarPanel.h"


#include <Imgui/imgui.h>
namespace IM {
	ToolbarPanel::ToolbarPanel()
	{
		_PlayIcon = Texture2D::Create("Resources/Icons/PlayIcon.png");
		_StopIcon = Texture2D::Create("Resources/Icons/StopIcon.png");
	}
	ToolbarPanel::ToolbarPanel(const EventCallbackFn& callback)
	{
		_EventCallback = callback;
		_PlayIcon = Texture2D::Create("Resources/Icons/PlayIcon.png");
		_StopIcon = Texture2D::Create("Resources/Icons/StopIcon.png");
	}
	void ToolbarPanel::OnImGuiRender(const SceneState& state) {

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		auto colors = ImGui::GetStyle().Colors;
		const auto& buttonHovered = colors[ImGuiCol_ButtonHovered];
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f));
		const auto& buttonActive = colors[ImGuiCol_ButtonActive];
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(buttonActive.x, buttonActive.y, buttonActive.z, 0.5f));

		ImGui::Begin("##toolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

		float size = ImGui::GetWindowHeight() - 4.0f;
		RefPtr<Texture2D> icon = state == SceneState::Stop ? _PlayIcon : _StopIcon;
		ImGui::SameLine((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));
		if (ImGui::ImageButton((ImTextureID)icon->GetID(), ImVec2(size, size), ImVec2(0,0), ImVec2(1,1), 0)) {
			SceneChangeEvent e;
			_EventCallback(e);
		}
		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(3);
		ImGui::End();
	}
}