#pragma once

#include "Core/Core.h"

namespace IM {
	class IMAGINE_API Input {
	public:
		inline static bool IsKeyPressed(int keycode) { return _Instance->IsKeyPressedImpl(keycode); }
		inline static bool IsMouseButtonPressed(int button) { return _Instance->IsMouseButtonPressedImpl(button); }
		inline static std::pair<float, float> GetMousePos() { return _Instance->GetMousePosImpl(); }
	protected:
		virtual bool IsKeyPressedImpl(int keycode) = 0;
		virtual bool IsMouseButtonPressedImpl(int button) = 0;
		virtual std::pair<float, float> GetMousePosImpl() = 0;
	private:
		static ScopePtr<Input> _Instance;
	};

}