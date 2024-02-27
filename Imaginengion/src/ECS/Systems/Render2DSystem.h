#pragma once

#include "System.h"
namespace IM {
	class Render2DSystem : public System {
	public:
		Render2DSystem() = default;
		Render2DSystem(size_t id)
			: System(id) {}
		Render2DSystem(size_t id, std::set<size_t> hashes)
			: System(id, hashes) {}
		~Render2DSystem() = default;
		void OnUpdate(ScopePtr<ComponentManager>& _ComponentManager, float dt) override;
	};
}