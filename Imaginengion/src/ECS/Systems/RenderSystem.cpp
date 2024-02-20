#include "impch.h"
#include "RenderSystem.h"

#include "ECS/Components.h"

#include "Renderer/Renderer.h"

namespace IM {
	void RenderSystem::OnUpdate(ScopePtr<ComponentManager>& _ComponentManager, float dt) {
		auto& group = System::GetGroup<C_Transform, C_SpriteRenderer>(_ComponentManager, _TypeHashes, _TypeID);
		for (auto entity : group) {
			auto [transform, sprite] = System::GetComponents<C_Transform, C_SpriteRenderer>(_ComponentManager, entity);
			Renderer::R2D::DrawRect(transform.GetTransform(), sprite.Color);
		}
	}
}