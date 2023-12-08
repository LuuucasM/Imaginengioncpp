#pragma once

#include "OrthographicCamera.h"

namespace IM {
	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false);
		~OrthographicCameraController();

		void OnUpdate(float dt);

		OrthographicCamera& GetCamera() { return _Camera; }
		const OrthographicCamera& GetCamera() const { return _Camera; }
	private:
		void OnMouseScrolled(float xOffset, float yOffset);
		void OnWindowResized(int width, int height);
	private:
		float _AspectRatio;
		float _ZoomLevel = 1.0f;
		OrthographicCamera _Camera;
		
		glm::vec3 _CameraPosition = {0.0f, 0.0f, 0.0f};
		float _CameraTranslationSpeed = 5.0f;

		bool _CanRotate;
		float _CameraRotation = 0.0f;
		float _CameraRotationSpeed = 30.0f;
	};
}
