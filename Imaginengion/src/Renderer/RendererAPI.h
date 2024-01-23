#pragma once

#include "VertexArray.h"

#include "glm/glm.hpp"

namespace IM {
	class RendererAPI {
	public:

		virtual ~RendererAPI() = default;
		enum class API {
			None = 0,
			OpenGL = 1
		};

		virtual void Init() = 0;
		virtual void SetViewport(int x, int y, size_t width, size_t height) = 0;
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const RefPtr<VertexArray>& vertexArray, uint32_t indexCount = 0) = 0;

		static API GetCurrentAPI() { return _API; }
		static ScopePtr<RendererAPI> Create();
	private:
		static API _API;
	};
}