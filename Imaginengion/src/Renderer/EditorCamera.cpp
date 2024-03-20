#include "impch.h"
#include "EditorCamera.h"

#include "Core/Input.h"

#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_QUAT_DATA_WXYZ 
#include <glm/gtx/quaternion.hpp>

namespace IM {
	EditorCamera::EditorCamera(float fov, float aspectRatio, float nearClip, float farClip)
		: _FOV(fov), _AspectRatio(aspectRatio), _NearClip(nearClip), _FarClip(farClip), _ProjectionMatrix(glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip))
	{
		UpdateView();
	}

	void EditorCamera::UpdateProjection()
	{
		_AspectRatio = _ViewportWidth / _ViewportHeight;
		_ProjectionMatrix = glm::perspective(glm::radians(_FOV), _AspectRatio, _NearClip, _FarClip);
	}

	void EditorCamera::UpdateView()
	{
		// m_Yaw = m_Pitch = 0.0f; // Lock the camera's rotation
		_Position = CalculatePosition();

		glm::quat orientation = GetOrientation();
		_ViewMatrix = glm::translate(glm::mat4(1.0f), _Position) * glm::toMat4(orientation);
		_ViewMatrix = glm::inverse(_ViewMatrix);
	}

	std::pair<float, float> EditorCamera::PanSpeed() const
	{
		float x = std::min(_ViewportWidth / 1000.0f, 2.4f); // max = 2.4f
		float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

		float y = std::min(_ViewportHeight / 1000.0f, 2.4f); // max = 2.4f
		float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

		return { xFactor, yFactor };
	}

	float EditorCamera::RotationSpeed() const
	{
		return 0.8f;
	}

	float EditorCamera::ZoomSpeed() const
	{
		float distance = _Distance * 0.2f;
		distance = std::max(distance, 0.0f);
		float speed = distance * distance;
		speed = std::min(speed, 100.0f); // max speed = 100
		return speed;
	}

	void EditorCamera::OnUpdate(float ts)
	{
		if (Input::IsKeyPressed(Key::LeftAlt))
		{
			glm::vec2 pos = Input::GetMousePos();
			const glm::vec2& mouse{ pos.x, pos.y };
			glm::vec2 delta = (mouse - _InitialMousePosition) * 0.003f;
			_InitialMousePosition = mouse;

			if (Input::IsMouseButtonPressed(Mouse::ButtonMiddle))
				MousePan(delta);
			else if (Input::IsMouseButtonPressed(Mouse::ButtonLeft))
				MouseRotate(delta);
			else if (Input::IsMouseButtonPressed(Mouse::ButtonRight))
				MouseZoom(delta.y);
		}

		UpdateView();
	}

	void EditorCamera::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(IMAGINE_BIND_EVENT(EditorCamera::OnMouseScroll));
	}

	bool EditorCamera::OnMouseScroll(MouseScrolledEvent& e)
	{
		float delta = e.GetYOffset() * 0.1f;
		MouseZoom(delta);
		UpdateView();
		return false;
	}

	void EditorCamera::MousePan(const glm::vec2& delta)
	{
		auto [xSpeed, ySpeed] = PanSpeed();
		_FocalPoint += -GetRightDirection() * delta.x * xSpeed * _Distance;
		_FocalPoint += GetUpDirection() * delta.y * ySpeed * _Distance;
	}

	void EditorCamera::MouseRotate(const glm::vec2& delta)
	{
		float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;
		_Yaw += yawSign * delta.x * RotationSpeed();
		_Pitch += delta.y * RotationSpeed();
	}

	void EditorCamera::MouseZoom(float delta)
	{
		_Distance -= delta * ZoomSpeed();
		if (_Distance < 1.0f)
		{
			_FocalPoint += GetForwardDirection();
			_Distance = 1.0f;
		}
	}

	glm::vec3 EditorCamera::GetUpDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	glm::vec3 EditorCamera::GetRightDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
	}

	glm::vec3 EditorCamera::GetForwardDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
	}

	glm::vec3 EditorCamera::CalculatePosition() const
	{
		return _FocalPoint - GetForwardDirection() * _Distance;
	}

	glm::quat EditorCamera::GetOrientation() const
	{
		return glm::quat(glm::vec3(-_Pitch, -_Yaw, 0.0f));
	}
}