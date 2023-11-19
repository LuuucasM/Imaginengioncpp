#pragma once

#include "Renderer/VertexArray.h"

namespace IM {
	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray() = default;

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer> vertexBuffer) override;
		virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer> indexBuffer) override;
	};
}