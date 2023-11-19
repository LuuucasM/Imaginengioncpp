#pragma once

#include "Renderer/VertexBuffer.h"

namespace IM {
	class OpenGLVertexBuffer : public VertexBuffer {
	public:
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		~OpenGLVertexBuffer();

		void Bind() const override;
		void Unbind() const override;

		inline void SetLayout(const BufferLayout& layout) override {
			_Layout = layout;
		}
		inline const BufferLayout& GetLayout() const override {
			return _Layout;
		};
	};
}