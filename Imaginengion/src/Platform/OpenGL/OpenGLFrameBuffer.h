#pragma once

#include "Renderer/FrameBuffer.h"

namespace IM {
	class OpenGLFrameBuffer : public FrameBuffer
	{
	public:
		OpenGLFrameBuffer(const FrameBufferSpecification& spec);
		~OpenGLFrameBuffer();

		void Invalidate();

		void Bind() override;
		void Unbind() override;

		void Resize(size_t width, size_t height) override;
	};
}
