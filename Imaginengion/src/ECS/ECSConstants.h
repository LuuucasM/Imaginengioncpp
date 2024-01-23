#pragma once
#include <glm/glm.hpp>
#include <cstdint>
#include <bitset>

using EntityID = std::uint32_t;
using ComponentType = std::uint32_t;

const uint32_t MAX_ENTITIES = 2000;
const uint32_t MAX_COMPONENTS = 10;

using ComponentMask = std::bitset<MAX_COMPONENTS>;

enum CollisionClasses {
	PADDLE,
	BALL,
	BRICK,
	WALL
};

enum CollisionShapes {
	RECT = 1 << 0,
	CIRCLE = 1 << 1
};

typedef void (*CollisionFunc)(EntityID, EntityID, glm::vec3);