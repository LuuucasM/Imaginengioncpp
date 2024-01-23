#pragma once

#include "ECS/ECSCoordinator.h"

namespace IM {
	class Scene {
		Scene();
		~Scene();
	private:
		ECSCoordinator _ECSCoordinator;
	};
}