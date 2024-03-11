#pragma once
#include "Component.h"

#include "glm/glm.hpp"
#include "Renderer/Texture.h"

namespace IM {
	struct C_SpriteRenderer : public Component {
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };
		RefPtr<Texture2D> Texture;
		float TilingFactor = 1.0f;

		C_SpriteRenderer() = default;
		C_SpriteRenderer(const C_SpriteRenderer&) = default;
		C_SpriteRenderer(const glm::vec4& color)
			: Color(color) {}
	};
}