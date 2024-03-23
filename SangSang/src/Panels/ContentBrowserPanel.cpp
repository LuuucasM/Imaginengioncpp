#include "ContentBrowserPanel.h"
#include "Imagine.h"

#include <Imgui/imgui.h>


namespace IM {


	//TODO: when to have a projects object which contains the main working folder relative to the project, then change this
	static std::filesystem::path _AssetDirectory = "assets";

	ContentBrowserPanel::ContentBrowserPanel()
	{
		_CurrentDirectory = _AssetDirectory;
	}

	void ContentBrowserPanel::OnImGuiRender()
	{
		ImGui::Begin("Content Browser");

		if (_CurrentDirectory != _AssetDirectory) {
			if (ImGui::Button("<-")) {
				_CurrentDirectory = _CurrentDirectory.parent_path();
			}
		}

		for (auto& directoryEntry : std::filesystem::directory_iterator(_CurrentDirectory)) {

			const auto& path = directoryEntry.path();

			if (directoryEntry.is_directory()) {
				if (ImGui::Button(path.filename().string().c_str())) {
					_CurrentDirectory /= path.filename();
				}
			}
			else {
				ImGui::Text("%s", path.filename().string().c_str());
			}
			
		}


		ImGui::End();
	}
}