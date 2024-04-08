#pragma once

#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace IM {
	struct C_Camera {
		enum class ProjectionType { Perspective = 0, Orthographic = 1 };
		glm::mat4 Projection = glm::mat4(1.0f);
		float OrthographicSize = 10.0f;
		float OrthographicNear = -1.0f, OrthographicFar = 1.0f;

		float PerspectiveFOV = glm::radians(45.0f);
		float PerspectiveicNear = 0.01f, PerspectiveFar = 1000.0f;

		float AspectRatio = 0.0f;

		bool bPrimary = true; //TODO: MOVE INTO SCENE INSTEAD OF EACH CAMERA OWNING WHICH IS PRIMARY 
		bool bFixedAspectRatio = false;
		ProjectionType Type = ProjectionType::Perspective;

		C_Camera() {
			RecalculateProjection();
		}
		C_Camera(const C_Camera&) = default;

		void SetOrthographic(float size, float nearClip, float farClip) {
			OrthographicSize = size;
			OrthographicNear = nearClip;
			OrthographicFar = farClip;
			RecalculateProjection();
		}

		void SetViewportSize(size_t width, size_t height) {
			if (height) {
				AspectRatio = (float)width / (float)height;
			}
			else {
				AspectRatio = 0.0f;
			}
			RecalculateProjection();
		}

		void RecalculateProjection() {
			if (Type == ProjectionType::Perspective) {
				Projection = glm::perspective(PerspectiveFOV, AspectRatio, PerspectiveicNear, PerspectiveFar);
			}
			else if (Type == ProjectionType::Orthographic) {
				float orthoLeft = -OrthographicSize * AspectRatio * 0.5f;
				float orthoRight = OrthographicSize * AspectRatio * 0.5f;
				float orthoBottom = -OrthographicSize * 0.5f;
				float orthoTop = OrthographicSize * 0.5f;
				Projection = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop,
					OrthographicNear, OrthographicFar);
			}
		}
	};
}