#include "ContentBrowserPanel.h"
#include "Imagine.h"

#include <Imgui/imgui.h>

namespace IM {


	//TODO: when to have a projects object which contains the main working folder relative to the project, then change this
	static std::filesystem::path _AssetDirectory = "assets";

	ContentBrowserPanel::ContentBrowserPanel()
	{
		_CurrentDirectory = _AssetDirectory;

		_FolderIcon = Texture2D::Create("Resources/Icons/ContentBrowser/FolderIcon.png");
		_FileIcon = Texture2D::Create("Resources/Icons/ContentBrowser/FileIcon.png");
		_BackArrow = Texture2D::Create("Resources/Icons/ContentBrowser/BackArrow.png");
	}

	void ContentBrowserPanel::OnImGuiRender()
	{
		ImGui::Begin("Content Browser");

		constexpr float padding = 8.0f;
		constexpr float thumbnailSize = 70.0f;
		float cellSize = thumbnailSize + padding;

		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(panelWidth / cellSize);
		if (columnCount < 1) {
			columnCount = 1;
		}

		ImGui::Columns(columnCount, 0, false);



		RefPtr<Texture2D> icon;
		if (_CurrentDirectory != _AssetDirectory) {
			icon = _BackArrow;
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			ImGui::ImageButton((ImTextureID)icon->GetID(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
			ImGui::PopStyleColor();

			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
				_CurrentDirectory = _CurrentDirectory.parent_path();
			}
			ImGui::TextWrapped("Back");
			ImGui::NextColumn();
		}

		int i = 0;
		for (auto& directoryEntry : std::filesystem::directory_iterator(_CurrentDirectory)) {

			auto& path = directoryEntry.path();
			std::string filenameString = path.filename().string();

			ImGui::PushID(filenameString.c_str());
			icon = directoryEntry.is_directory() ? _FolderIcon : _FileIcon;
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			ImGui::ImageButton((ImTextureID)icon->GetID(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
			if (ImGui::BeginDragDropSource()) {
				std::string relativePathString = std::filesystem::relative(path, _AssetDirectory).string();
				auto itemPath = relativePathString.c_str();
				ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, strlen(itemPath)+1);
				ImGui::EndDragDropSource();
			}

			ImGui::PopStyleColor();

			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
				if (directoryEntry.is_directory()) {
					_CurrentDirectory /= path.filename();
				}
			}
			ImGui::TextWrapped(filenameString.c_str());

			ImGui::NextColumn();
			
			ImGui::PopID();
		}

		ImGui::Columns(1);

		ImGui::End();
	}
}