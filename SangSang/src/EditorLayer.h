#pragma once

#include "Imagine.h"
#include "Panels/Render2DStatsPanel.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/ContentBrowserPanel.h"
#include "Panels/PropertiesPanel.h"
#include "Panels/ToolbarPanel.h"
#include "Renderer/EditorCamera.h"

namespace IM {
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		~EditorLayer() = default;
		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate(float dt) override;
		void OnImguiRender() override;
		void OnEvent(Event& e) override;
	private:
		bool OnKeyPressed(KeyPressedEvent& e);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);

		void NewScene();
		void OpenScene();
		void OpenScene(const std::filesystem::path& path);
		void SerializeScene(RefPtr<Scene> scene, std::filesystem::path& path);
		void SaveScene();
		void SaveSceneAs();

		bool OnSceneChange(SceneChangeEvent& e);
		void OnScenePlay();
		void OnSceneStop();

		void  OnDuplicateEntity();
	private:
		RefPtr<FrameBuffer> _FrameBuffer;

		RefPtr<Scene> _ActiveScene;
		RefPtr<Scene> _EditorScene, _RuntimeScene;

		SceneState _SceneState = SceneState::Stop;

		std::filesystem::path _EditorScenePath;

		Entity _HoveredEntity;

		bool _PrimaryCamera = true;

		EditorCamera _EditorCamera;

		bool _bViewportFocus = false, _bViewportHovered = false;
		glm::vec2 _ViewportSize = glm::vec2(0.0f);
		glm::vec2 _ViewportBounds[2];

		int _GizmoType = -1;

		//Panels
		RefPtr<Render2DStatsPanel> _Render2DStatsPanel;
		RefPtr<SceneHierarchyPanel> _SceneHierarchyPanel;
		RefPtr<PropertiesPanel> _PropertiesPanel;
		RefPtr<ContentBrowserPanel> _ContentBrowserPanel;
		RefPtr<ToolbarPanel> _ToolbarPanel;
	};
}