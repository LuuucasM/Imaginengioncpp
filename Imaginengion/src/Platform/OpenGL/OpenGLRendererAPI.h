#pragma once

#include "Renderer/RendererAPI.h"

namespace IM {
	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		void Init() override;
		void SetViewport(int x, int y, size_t width, size_t height) override;

		void SetClearColor(const glm::vec4& color) override;
		void Clear() override;

		void DrawIndexed(const RefPtr<VertexArray>& vertexArray, uint32_t indexCount = 0) override;
	};
}


