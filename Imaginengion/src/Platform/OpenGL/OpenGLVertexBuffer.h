#pragma once

#include "Renderer/VertexBuffer.h"

namespace IM {
	class OpenGLVertexBuffer : public VertexBuffer {
	public:
		OpenGLVertexBuffer(size_t size);
		OpenGLVertexBuffer(float* vertices, size_t size);
		~OpenGLVertexBuffer();

		void Bind() const override;
		void Unbind() const override;

		void SetLayout(const BufferLayout& layout) override {
			_Layout = layout;
		}
		const BufferLayout& GetLayout() const override {
			return _Layout;
		};

		void SetData(const void* data, size_t size) override;
	};
}