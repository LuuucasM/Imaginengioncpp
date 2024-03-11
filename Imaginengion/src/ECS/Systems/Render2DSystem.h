#pragma once

#include "System.h"
namespace IM {
	class Render2DSystem : public System {
	public:
		Render2DSystem() = default;
		~Render2DSystem() = default;
		void OnUpdate(ScopePtr<ComponentManager>& _ComponentManager, float dt) override;
	};
}