#pragma once

#include "Imagine.h"
#include "../Panels/SceneHierarchyPanel.h"
#include "../Panels//PropertiesPanel.h"

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

		void NewScene();
		void OpenScene();
		void SaveAsScene();
	private:
		OrthographicCameraController _CameraController;

		RefPtr<Shader> _Shader;
		RefPtr<VertexArray> _VertexArray;
		glm::vec4 _SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
		RefPtr<FrameBuffer> _FrameBuffer;

		RefPtr<Scene> _ActiveScene;
		Entity _SquareEntity;
		Entity _CameraEntity;
		Entity _CameraEntity2;

		bool _PrimaryCamera = true;

		RefPtr<Texture2D> _Texture;
		RefPtr<Texture2D> _TextureZealot;

		bool _bViewportFocus = false, _bViewportHovered = false;
		glm::vec2 _ViewportSize = glm::vec2(0.0f);

		int _GizmoType = -1;

		RefPtr<SceneHierarchyPanel> _SceneHierarchyPanel;
		RefPtr<PropertiesPanel> _PropertiesPanel;

		float _FPS = 0.0f;
	};
}