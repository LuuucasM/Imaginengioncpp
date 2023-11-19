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

		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer> vertexBuffer) = 0;
		virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer> indexBuffer) = 0;

		static VertexArray* Create();

		inline const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const {
			return _VertexBuffers;
		}
		inline const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const {
			return _IndexBuffer;
		}

	protected:
		uint32_t _ArrayID;
		std::vector<std::shared_ptr<VertexBuffer>> _VertexBuffers;
		std::shared_ptr<IndexBuffer> _IndexBuffer;
	};
}
