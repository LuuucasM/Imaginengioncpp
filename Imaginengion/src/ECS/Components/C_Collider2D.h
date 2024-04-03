#pragma once

#include <glm/glm.hpp>

struct C_Collider2D {

	glm::vec2 _Offset = { 0.0f, 0.0f };
	glm::vec2 _Size = { 0.5f, 0.5f };

	//TODO: move this into a physics material at some point maybe
	float _Density = 1.0f;
	float _Friction = 0.5f;
	float _Restitution = 0.0f;
	float _RestitutionThreshold = 0.5f;

	C_Collider2D() = default;
	C_Collider2D(const C_Collider2D&) = default;

	void* RuntimeFixture = nullptr;
};