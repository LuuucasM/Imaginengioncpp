#pragma once

#include "Renderer/VertexArray.h"

namespace IM {
	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		~OpenGLVertexArray();

		void Bind() const override;
		void Unbind() const override;

		void AddVertexBuffer(const RefPtr<VertexBuffer> vertexBuffer) override;
		void SetIndexBuffer(const RefPtr<IndexBuffer> indexBuffer) override;
	};
}