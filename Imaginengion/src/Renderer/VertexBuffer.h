#pragma once

#include "BufferLayout.h"

namespace IM {
	class VertexBuffer {
	public:
		virtual ~VertexBuffer() = default;

		//virtual void SetData() = 0;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetLayout(const BufferLayout& layout) = 0;
		virtual const BufferLayout& GetLayout() const = 0;

		virtual void SetData(const void* data, size_t size) = 0;

		static RefPtr<VertexBuffer> Create(size_t size);
		static RefPtr<VertexBuffer> Create(float *verticies, size_t size);
		
	protected:
		uint32_t _BufferID;
		BufferLayout _Layout;
	};
}