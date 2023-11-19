#pragma once

#include "Renderer/IndexBuffer.h"

namespace IM {
	class OpenGLIndexBuffer : public IndexBuffer {
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		~OpenGLIndexBuffer();

		void Bind() const override;
		void Unbind() const override;
	private:
		uint32_t _BufferID;
	};
}