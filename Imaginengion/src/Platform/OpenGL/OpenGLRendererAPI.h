#pragma once

#include "Renderer/RendererAPI.h"

namespace IM {
	class OpenGLRendererAPI : public RendererAPI
	{
		void Init() override;
		void SetClearColor(const glm::vec4& color) override;
		void Clear() override;

		void DrawIndexed(const RefPtr<VertexArray>& vertexArray) override;
	};
}


