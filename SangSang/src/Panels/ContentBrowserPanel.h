#pragma once

#include <filesystem>

namespace IM {
	class ContentBrowserPanel
	{
	public:
		ContentBrowserPanel();
		~ContentBrowserPanel() = default;
		void OnImGuiRender();
	private:
		std::filesystem::path _CurrentDirectory;
	};
}
