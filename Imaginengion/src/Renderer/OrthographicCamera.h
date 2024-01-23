#pragma once

#include "glm/glm.hpp"

namespace IM {
	class OrthographicCamera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top);

		void SetPosition(const glm::vec3& position) { 
			_Position = position; 
			RecalculatedViewMatrix();
		}
		void SetRotation(float rotation) { 
			_Rotation = rotation; 
			RecalculatedViewMatrix();
		}

		void SetProjection(float left, float right, float bottom, float top);

		const glm::vec3& GetPosition() const { return _Position; }
		float GetRotation() const { return _Rotation; }
		const glm::mat4& GetPojectionMatrix() const { return _ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return _ProjectionMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return _ViewProjectionMatrix; }
	private:
		void RecalculatedViewMatrix();

		glm::mat4 _ProjectionMatrix;
		glm::mat4 _ViewMatrix;
		glm::mat4 _ViewProjectionMatrix;

		glm::vec3 _Position = glm::vec3(0.0f);
		float _Rotation = 0.0f;
	};
}