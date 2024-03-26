#pragma once

#include "Renderer/Texture.h"

namespace IM {
	namespace {
		enum class SceneState {
			Stop = 0,
			Play = 1
		};
	}
	class ToolbarPanel
	{
	public:
		ToolbarPanel();
		~ToolbarPanel() = default;
		//ToolbarPanel();

		void OnImGuiRender();

		SceneState GetSceneState() const { return _SceneState; }
	private:
		SceneState _SceneState = SceneState::Stop;
		RefPtr<Texture2D> _PlayIcon;
		RefPtr<Texture2D> _StopIcon;
	};
}