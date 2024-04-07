#pragma once

#include "RendererAPI.h"

namespace IM {
	class RenderCommand {
	public:
		static void Init() { 
			_RendererAPI->Init(); 
		}

		static void SetViewport(int x, int y, size_t width, size_t height) {
			_RendererAPI->SetViewport(x, y, width, height);
		}

		static void SetClearColor(const glm::vec4& color) {
			_RendererAPI->SetClearColor(color);
		}
		static void Clear() {
			_RendererAPI->Clear();
		}
		static void DrawIndexed(const RefPtr<VertexArray>& vertexArray, uint32_t indexCount = 0) {
			_RendererAPI->DrawIndexed(vertexArray, indexCount);
		}
		static void DrawLines(const RefPtr<VertexArray>& vertexArray, uint32_t vertexCount) {
			_RendererAPI->DrawLines(vertexArray, vertexCount);
		}

		static void SetEditorLineThickness(float thickness) {
			_RendererAPI->SetEditorLineThickness(thickness);
		}
	private:
		static ScopePtr<RendererAPI> _RendererAPI;
	};
}