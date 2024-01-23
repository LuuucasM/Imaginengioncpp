#include "impch.h"
#include "Scene.h"

#include <glm/glm.hpp>

namespace IM {
	Scene::Scene()
	{
		struct TransformComponent {
			glm::mat4 Transform;

			TransformComponent() = default;
			~TransformComponent() = default;
			TransformComponent(const TransformComponent&) = default;
			TransformComponent(const glm::mat4& transform)
				: Transform(transform) {}

			operator glm::mat4& () { return Transform; }
			operator const glm::mat4& () { return Transform; }
		};
		EntityID entity = _ECSCoordinator.CreateEntity();
	}
	Scene::~Scene()
	{
	}
}