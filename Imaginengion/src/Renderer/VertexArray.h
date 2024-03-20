#pragma once

#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include <memory>

namespace IM {
	class VertexArray
	{
	public:
		VertexArray();
		~VertexArray();

		void Bind() const;
		void Unbind() const;

		void AddVertexBuffer(const RefPtr<VertexBuffer> vertexBuffer);
		void SetIndexBuffer(const RefPtr<IndexBuffer> indexBuffer);

		static RefPtr<VertexArray> Create() {
			return CreateRefPtr<VertexArray>();
		};

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
