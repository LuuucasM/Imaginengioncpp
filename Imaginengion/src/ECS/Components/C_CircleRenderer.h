#pragma once

#include "glm/glm.hpp"

namespace IM {
	struct C_CircleRenderer {
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };
		float Radius = 0.5f;
		float Thickness = 1.0f;
		float Fade = 0.005f;

		C_CircleRenderer() = default;
		C_CircleRenderer(const C_CircleRenderer&) = default;
		C_CircleRenderer(const glm::vec4& color)
			: Color(color) {}
	};
}
