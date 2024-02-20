#pragma once

#include "System.h"
namespace IM {
	class RenderSystem : public System {
	public:
		RenderSystem() = default;
		RenderSystem(size_t id)
			: System(id) {}
		~RenderSystem() = default;
		void OnUpdate(ScopePtr<ComponentManager>& _ComponentManager, float dt) override;
	};
}