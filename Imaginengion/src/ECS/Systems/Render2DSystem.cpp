#include "impch.h"
#include "Render2DSystem.h"

#include "ECS/Components.h"

#include "Renderer/Renderer.h"

namespace IM {
	void Render2DSystem::OnUpdate(ScopePtr<ComponentManager>& _ComponentManager, float dt) {

		auto& group = System::GetGroup<C_Transform, C_SpriteRenderer>(_ComponentManager);
		for (auto entity : group) {
			auto [transform, sprite] = System::GetComponents<C_Transform, C_SpriteRenderer>(_ComponentManager, entity);
			Renderer::R2D::DrawSprite(transform.GetTransform(), sprite, entity);
		}
	}
}