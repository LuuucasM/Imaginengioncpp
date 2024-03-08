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
		uint32_t ReadPixel(uint32_t attachmentIndex, int x, int y) override;
		void ClearColorAttachment(uint32_t attachmentIndex, uint32_t value) override;
	};
}
