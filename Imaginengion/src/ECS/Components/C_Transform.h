#pragma once

#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include <iostream>

namespace IM {
	struct C_Transform {

		glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

		C_Transform() = default;
		C_Transform(glm::vec3 trans)
			: Translation(trans) {}
		C_Transform(const C_Transform&) = default;

		const glm::mat4& GetTransform() {
			if (_CacheTranslation != Translation || _CacheRotation != Rotation || _CacheScale != Scale) {
				_CacheTranslation = Translation;
				_CacheRotation = Rotation;
				_CacheScale = Scale;

				glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));
				_Transform = glm::translate(glm::mat4(1.0f), Translation) * rotation * glm::scale(glm::mat4(1.0f), Scale);
			}
			return _Transform;
		}

	private:
		glm::vec3 _CacheTranslation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 _CacheRotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 _CacheScale = { 1.0f, 1.0f, 1.0f };

		glm::mat4 _Transform = glm::mat4(1.0f);
		
		bool _Dirty = true; //how to use this so i dont have to use CacheTranslation, etc
	};
}