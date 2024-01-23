#pragma once

#include "OrthographicCamera.h"
#include "Events/SystemEvents.h"

namespace IM {
	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false);
		~OrthographicCameraController() = default;

		void OnUpdate(float dt);
		void OnEvent(Event& e);

		OrthographicCamera& GetCamera() { return _Camera; }
		const OrthographicCamera& GetCamera() const { return _Camera; }

		float GetZoomLevel() const { return _ZoomLevel; }
		void SetZoomLevel(float level) { _ZoomLevel = level; RecalculateProjection(); }

		void OnResize(size_t width, size_t height);
	private:
		void RecalculateProjection();
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
	private:
		float _AspectRatio;
		float _ZoomLevel = 1.0f;
		OrthographicCamera _Camera;
		
		glm::vec3 _CameraPosition = {0.0f, 0.0f, 0.0f};
		float _CameraTranslationSpeed = 5.0f;

		bool _bCanRotate;
		float _CameraRotation = 0.0f;
		float _CameraRotationSpeed = 30.0f;
	};
}
