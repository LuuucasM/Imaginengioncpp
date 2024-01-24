#pragma once

#include "Core/Core.h"
#include "Renderer/Texture.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
namespace IM {
	struct C_Transform {
		glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

		C_Transform() = default;
		C_Transform(const C_Transform&) = default;
		glm::mat4 GetTransform() const {
			glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

			return glm::translate(glm::mat4(1.0f), Translation) * rotation * glm::scale(glm::mat4(1.0f), Scale);
		}
	};

	struct C_SpriteRenderer {
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };
		RefPtr<Texture2D> Texture;
		float TilingFactor = 1.0f;

		C_SpriteRenderer() = default;
		C_SpriteRenderer(const C_SpriteRenderer&) = default;
	};
}