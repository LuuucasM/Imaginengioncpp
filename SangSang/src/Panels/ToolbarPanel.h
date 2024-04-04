#pragma once


#include "Scene/Scene.h"
#include "Renderer/Texture.h"
#include "Events/EditorEvents.h"

namespace IM {

	enum class SceneState {
		Stop = 0,
		Play = 1
	};
	class ToolbarPanel
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;
		ToolbarPanel();
		ToolbarPanel(const EventCallbackFn& callback);
		~ToolbarPanel() = default;

		void OnImGuiRender(const SceneState& state);

	private:
		SceneState _SceneState = SceneState::Stop;
		RefPtr<Texture2D> _PlayIcon;
		RefPtr<Texture2D> _StopIcon;
		EventCallbackFn _EventCallback;
	};
}