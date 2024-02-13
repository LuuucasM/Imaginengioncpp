#pragma once

#include "Shader.h"

namespace IM {
	class VertexBuffer {
	public:
		virtual ~VertexBuffer() = default;

		//virtual void SetData() = 0;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

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

		virtual void SetData(const void* data, size_t size) = 0;

		static RefPtr<VertexBuffer> Create(size_t size);
		static RefPtr<VertexBuffer> Create(float *verticies, size_t size);
		
	protected:
		uint32_t _BufferID;
		std::vector<BufferElement> _Layout;
		uint32_t _Stride;
	};
}