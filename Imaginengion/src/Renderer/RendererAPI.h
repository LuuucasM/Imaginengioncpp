#pragma once

#include "VertexArray.h"

#include "glm/glm.hpp"

namespace IM {
	class RendererAPI {
	public:
		enum class API {
			None = 0,
			OpenGL = 1
		};

		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const RefPtr<VertexArray>& vertexArray) = 0;

		inline static API GetCurrentAPI() { return _API; }
	private:
		static API _API;
	};
}