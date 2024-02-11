#pragma once

#include "Core/Core.h"
#include "Renderer/Texture.h"
#include "Scene/ScriptClass.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include <type_traits>

namespace IM {
	struct C_Transform {
		glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

		C_Transform() = default;
		C_Transform(glm::vec3 trans) 
			: Translation(trans){}
		C_Transform(const C_Transform&) = default;

		glm::mat4 GetTransform() const {
			glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));
			return glm::translate(glm::mat4(1.0f), Translation) * rotation * glm::scale(glm::mat4(1.0f), Scale);
		}
	};

	struct C_Name {
		std::string Name;
		C_Name() = default;
		C_Name(const C_Name&) = default;
		C_Name(const std::string& name)
			: Name(name){}
	};

	struct C_ColorRenderer {
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };

		C_ColorRenderer() = default;
		C_ColorRenderer(const C_ColorRenderer&) = default;
		C_ColorRenderer(const glm::vec4& color)
			: Color(color) {}
	};

	struct C_SpriteRenderer {
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };
		RefPtr<Texture2D> Texture;
		float TilingFactor = 1.0f;

		C_SpriteRenderer() = default;
		C_SpriteRenderer(const C_SpriteRenderer&) = default;
		C_SpriteRenderer(const glm::vec4& color)
			: Color(color){}
	};

	struct C_Camera {
		enum class ProjectionType {Perspective = 0, Orthographic = 1};
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

	struct C_NativeScript {
		ScriptClass* Instance = nullptr;
		ScriptClass* (*CreateScript)();
		void (*DestroyScript)(C_NativeScript*);

		template<typename T>
		void Bind() {
			CreateScript = []() { return static_cast<ScriptClass*>(new T()); };
			DestroyScript = [](C_NativeScript *nativeScript) {delete nativeScript->Instance; nativeScript->Instance = nullptr; };
		}
	};
}