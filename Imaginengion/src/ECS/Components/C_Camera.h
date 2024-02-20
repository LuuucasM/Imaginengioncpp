#pragma once

#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace IM {
	struct C_Camera {
		enum class ProjectionType { Perspective = 0, Orthographic = 1 };
		glm::mat4 Projection = glm::mat4(1.0f);
		float _OrthographicSize = 10.0f;
		float _OrthographicNear = -1.0f, _OrthographicFar = 1.0f;

		float _PerspectiveFOV = glm::radians(45.0f);
		float _PerspectiveicNear = 0.01f, _PerspectiveFar = 1000.0f;

		float _AspectRatio = 0.0f;

		bool _bPrimary = true; //TODO: MOVE INTO SCENE INSTEAD OF EACH CAMERA OWNING WHICH IS PRIMARY 
		bool _bFixedAspectRatio = false;
		ProjectionType _ProjectionType = ProjectionType::Perspective;

		C_Camera() {
			RecalculateProjection();
		}
		C_Camera(const C_Camera&) = default;

		void SetOrthographic(float size, float nearClip, float farClip) {
			_OrthographicSize = size;
			_OrthographicNear = nearClip;
			_OrthographicFar = farClip;
			RecalculateProjection();
		}

		void SetViewportSize(size_t width, size_t height) {
			if (height) {
				_AspectRatio = (float)width / (float)height;
			}
			else {
				_AspectRatio = 0.0f;
			}
			RecalculateProjection();
		}

		void RecalculateProjection() {
			if (_ProjectionType == ProjectionType::Perspective) {
				Projection = glm::perspective(_PerspectiveFOV, _AspectRatio, _PerspectiveicNear, _PerspectiveFar);
			}
			else if (_ProjectionType == ProjectionType::Orthographic) {
				float orthoLeft = -_OrthographicSize * _AspectRatio * 0.5f;
				float orthoRight = _OrthographicSize * _AspectRatio * 0.5f;
				float orthoBottom = -_OrthographicSize * 0.5f;
				float orthoTop = _OrthographicSize * 0.5f;
				Projection = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop,
					_OrthographicNear, _OrthographicFar);
			}
		}
	};
}