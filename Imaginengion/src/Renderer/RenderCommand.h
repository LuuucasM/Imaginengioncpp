#pragma once

#include "RendererAPI.h"

namespace IM {
	class RenderCommand {
	public:
		inline static void Init() { 
			_RendererAPI->Init(); 
		}

		inline static void SetViewport(int x, int y, int width, int height) {
			_RendererAPI->SetViewport(x, y, width, height);
		}

		inline static void SetClearColor(const glm::vec4& color) {
			_RendererAPI->SetClearColor(color);
		}
		inline static void Clear() {
			_RendererAPI->Clear();
		}
		inline static void DrawIndexed(const RefPtr<VertexArray>& vertexArray) {
			_RendererAPI->DrawIndexed(vertexArray);
		}
	private:
		static ScopePtr<RendererAPI> _RendererAPI;
	};
}