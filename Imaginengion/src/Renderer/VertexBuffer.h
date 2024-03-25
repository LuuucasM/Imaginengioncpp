#pragma once

#include "Shader.h"

namespace IM {
	class VertexBuffer {
	public:
		VertexBuffer(size_t size);
		VertexBuffer(float* verticies, size_t size);
		~VertexBuffer();

		void Bind() const;
		void Unbind() const;

		void SetData(const void* data, size_t size);

		void SetLayout(const std::vector<BufferElement>& layout) {
			_Layout = layout;
		}

		void SetStride(const uint32_t stride) {
			_Stride = stride;
		}

		const std::vector<BufferElement>& GetLayout() {
			return _Layout;
		}
		const uint32_t GetStride() {
			return _Stride;
		}

		static RefPtr<VertexBuffer> Create(size_t size) {
			return CreateRefPtr<VertexBuffer>(size);
		}
		static RefPtr<VertexBuffer> Create(float* verticies, size_t size) {
			return CreateRefPtr<VertexBuffer>(verticies, size);
		}
		
	protected:
		uint32_t _BufferID;
		std::vector<BufferElement> _Layout;
		uint32_t _Stride;
	};
}