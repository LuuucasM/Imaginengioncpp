#pragma once

#include "RendererAPI.h"

namespace IM {
	class RenderCommand {
	public:
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
		static RendererAPI* _RendererAPI;
	};
}