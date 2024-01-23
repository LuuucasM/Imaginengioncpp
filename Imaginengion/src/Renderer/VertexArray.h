#pragma once

#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include <memory>

namespace IM {
	class VertexArray
	{
	public:
		virtual ~VertexArray() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void AddVertexBuffer(const RefPtr<VertexBuffer> vertexBuffer) = 0;
		virtual void SetIndexBuffer(const RefPtr<IndexBuffer> indexBuffer) = 0;

		static RefPtr<VertexArray> Create();

		const std::vector<RefPtr<VertexBuffer>>& GetVertexBuffers() const {
			return _VertexBuffers;
		}
		const RefPtr<IndexBuffer>& GetIndexBuffer() const {
			return _IndexBuffer;
		}

	protected:
		uint32_t _ArrayID;
		std::vector<RefPtr<VertexBuffer>> _VertexBuffers;
		RefPtr<IndexBuffer> _IndexBuffer;
	};
}
