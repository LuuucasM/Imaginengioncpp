#pragma once

#include "Renderer/Texture.h"

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

		RefPtr<Texture2D> _FolderIcon;
		RefPtr<Texture2D> _FileIcon;
		RefPtr<Texture2D> _BackArrow;
	};
}
