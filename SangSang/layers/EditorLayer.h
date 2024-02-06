#pragma once

#include "Imagine.h"
#include "../Panels/SceneHierarchyPanel.h"

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
		OrthographicCameraController _CameraController;

		//TEMP
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

		SceneHierarchyPanel _SceneHierarchyPanel;
	};
}