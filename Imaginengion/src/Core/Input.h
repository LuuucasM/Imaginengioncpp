#pragma once

#include "Core/Core.h"
#include "Keycodes.h"
#include "MouseCodes.h"

#include <glm/glm.hpp>

namespace IM {
	class Input {

	public:
		static bool IsKeyPressed(KeyCode key);
		static bool IsMouseButtonPressed(MouseCode button);
		static glm::vec2 GetMousePos();
	};

}