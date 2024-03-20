#include "impch.h"

#ifdef IMAGINE_OPENGL
#include "Renderer/UniformBuffer.h"

#include <glad/glad.h>

namespace IM {
	UniformBuffer::UniformBuffer(uint32_t size, uint32_t binding) {
		glCreateBuffers(1, &_BufferID);
		glNamedBufferData(_BufferID, size, nullptr, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, binding, _BufferID);
	}
	UniformBuffer::~UniformBuffer() {
		glDeleteBuffers(1, &_BufferID);
	}
	void UniformBuffer::SetData(const void* data, uint32_t size, uint32_t offset) {
		glNamedBufferSubData(_BufferID, offset, size, data);
	}
}
#endif