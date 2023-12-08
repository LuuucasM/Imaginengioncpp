#include "impch.h"
#include "OrthographicCameraController.h"

#include "Core/Input.h"
#include "Core/Keycodes.h"

#include "Core/Application.h"

namespace IM {
	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		: _AspectRatio(aspectRatio), _Camera(-_AspectRatio * _ZoomLevel, _AspectRatio * _ZoomLevel, -_ZoomLevel, _ZoomLevel), _CanRotate(rotation){
		auto& application = Application::Get();
		auto& window = application.GetWindow();

		window.MouseScrolledEvent.AddListener(this, &OrthographicCameraController::OnMouseScrolled);
		window.WindowResizeEvent.AddListener(this, &OrthographicCameraController::OnWindowResized);
	}
	OrthographicCameraController::~OrthographicCameraController()
	{
	}
	void OrthographicCameraController::OnUpdate(float dt)
	{
		if (Input::IsKeyPressed(IMAGINE_KEY_A))
			_CameraPosition.x -= _CameraTranslationSpeed * dt;
		if (Input::IsKeyPressed(IMAGINE_KEY_D))
			_CameraPosition.x += _CameraTranslationSpeed * dt;
		if (Input::IsKeyPressed(IMAGINE_KEY_W))
			_CameraPosition.y += _CameraTranslationSpeed * dt;
		if (Input::IsKeyPressed(IMAGINE_KEY_S))
			_CameraPosition.y -= _CameraTranslationSpeed * dt;

		if (_CanRotate) {
			if (Input::IsKeyPressed(IMAGINE_KEY_Q))
				_CameraRotation += _CameraRotationSpeed * dt;
			if (Input::IsKeyPressed(IMAGINE_KEY_E))
				_CameraRotation -= _CameraRotationSpeed * dt;

			_Camera.SetRotation(_CameraRotation);
		}

		_Camera.SetPosition(_CameraPosition);
	}
	void OrthographicCameraController::OnMouseScrolled(float xOffset, float yOffset)
	{
		_ZoomLevel -= yOffset * 0.25;
		_Camera.SetProjection(-_AspectRatio * _ZoomLevel, _AspectRatio * _ZoomLevel, -_ZoomLevel, _ZoomLevel);
	}
	void OrthographicCameraController::OnWindowResized(int width, int height)
	{
		_AspectRatio = (float)width / (float)height;
		_Camera.SetProjection(-_AspectRatio * _ZoomLevel, _AspectRatio * _ZoomLevel, -_ZoomLevel, _ZoomLevel);
	}
}