#pragma once

#pragma once
#include <glm/glm.hpp>

struct C_CircleCollider2D {

	glm::vec2 Offset = { 0.0f, 0.0f };
	float Radius = 0.5f;

	//TODO: move this into a physics material at some point maybe
	float Density = 1.0f;
	float Friction = 0.5f;
	float Restitution = 0.0f;
	float RestitutionThreshold = 0.5f;

	void* RuntimeFixture = nullptr;

	C_CircleCollider2D() = default;
	C_CircleCollider2D(const C_CircleCollider2D&) = default;
};