#include "impch.h"
#include "Scene.h"

#include "ECS/Components.h"

#include <glm/glm.hpp>

namespace IM {
	Scene::Scene()
	{
		EntityID entity = _ECSCoordinator.CreateEntity();
		_ECSCoordinator.AddComponent<C_Transform>(entity, {});
	}
	Scene::~Scene()
	{
	}
}