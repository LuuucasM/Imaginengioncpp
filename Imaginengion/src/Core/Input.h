#pragma once

#include "Core/Core.h"
#include "Keycodes.h"
#include "MouseCodes.h"

namespace IM {
	class Input {

	public:
		static bool IsKeyPressed(KeyCode key);
		static bool IsMouseButtonPressed(MouseCode button);
		static std::pair<float, float> GetMousePos();
	};

}