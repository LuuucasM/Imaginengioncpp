#pragma once

#include "VertexArray.h"

#include "glm/glm.hpp"

namespace IM {
	class RendererAPI {
	public:
		RendererAPI() = default;
		virtual ~RendererAPI() = default;
		enum class API {
			None = 0,
			OpenGL = 1
		};

		void Init();
		void SetViewport(int x, int y, size_t width, size_t height);
		void SetClearColor(const glm::vec4& color);
		void Clear();

		void DrawIndexed(const RefPtr<VertexArray>& vertexArray, uint32_t indexCount = 0);

		static API GetCurrentAPI() { return _API; }
		static ScopePtr<RendererAPI> Create() {
			return ScopePtr<RendererAPI>();
		};
	private:
		static API _API;
	};
}