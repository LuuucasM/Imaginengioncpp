#include "impch.h"
#include "OrthographicCameraController.h"

#include "Core/Input.h"

#include "Core/Application.h"

namespace IM {
	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		: _AspectRatio(aspectRatio), _Camera(-_AspectRatio * _ZoomLevel, _AspectRatio * _ZoomLevel, -_ZoomLevel, _ZoomLevel), _bCanRotate(rotation){
		auto& application = Application::Get();
		auto& window = application.GetWindow();
	}
	void OrthographicCameraController::OnUpdate(float dt)
	{

		IMAGINE_PROFILE_FUNCTION();

		if (Input::IsKeyPressed(Key::A))
			_CameraPosition.x -= _CameraTranslationSpeed * dt;
		if (Input::IsKeyPressed(Key::D))
			_CameraPosition.x += _CameraTranslationSpeed * dt;
		if (Input::IsKeyPressed(Key::W))
			_CameraPosition.y += _CameraTranslationSpeed * dt;
		if (Input::IsKeyPressed(Key::S))
			_CameraPosition.y -= _CameraTranslationSpeed * dt;

		if (_bCanRotate) {
			if (Input::IsKeyPressed(Key::Q))
				_CameraRotation += _CameraRotationSpeed * dt;
			if (Input::IsKeyPressed(Key::E))
				_CameraRotation -= _CameraRotationSpeed * dt;
			_Camera.SetRotation(_CameraRotation);
		}

		_Camera.SetPosition(_CameraPosition);
	}

	void OrthographicCameraController::OnEvent(Event& e)
	{
		IMAGINE_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(IMAGINE_BIND_EVENT(OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(IMAGINE_BIND_EVENT(OnWindowResized));
	}

	void OrthographicCameraController::RecalculateProjection()
	{
		_Camera.SetProjection(-_AspectRatio * _ZoomLevel, _AspectRatio * _ZoomLevel, -_ZoomLevel, _ZoomLevel);
	}

	void OrthographicCameraController::OnResize(size_t width, size_t height)
	{
		_AspectRatio = (float)width / (float)height;
		RecalculateProjection();
	}
	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{

		IMAGINE_PROFILE_FUNCTION();

		_ZoomLevel -= e.GetYOffset() * 0.25f;
		RecalculateProjection();
		return false;
	}
	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
	{

		IMAGINE_PROFILE_FUNCTION();

		OnResize(e.GetWidth(), e.GetHeight());
		return false;
	}
}