#pragma once


#include "Scene/Scene.h"
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
		ToolbarPanel(const WeakPtr<Scene>& scene);
		~ToolbarPanel() = default;
		
		void SetContext(const WeakPtr<Scene>& scene);

		void OnImGuiRender();

		SceneState GetSceneState() const { return _SceneState; }
	private:
		SceneState _SceneState = SceneState::Stop;
		RefPtr<Texture2D> _PlayIcon;
		RefPtr<Texture2D> _StopIcon;

		WeakPtr<Scene> _Context;
	};
}