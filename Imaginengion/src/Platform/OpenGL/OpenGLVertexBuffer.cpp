#include "impch.h"

#ifdef IMAGINE_OPENGL
#include "Renderer/VertexBuffer.h"

#include "glad/glad.h"

namespace IM {
	VertexBuffer::VertexBuffer(size_t size) {
		IMAGINE_PROFILE_FUNCTION();

		glCreateBuffers(1, &_BufferID);
		glBindBuffer(GL_ARRAY_BUFFER, _BufferID);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}
	VertexBuffer::VertexBuffer(float* vertices, size_t size) {
		IMAGINE_PROFILE_FUNCTION();

		glCreateBuffers(1, &_BufferID);
		glBindBuffer(GL_ARRAY_BUFFER, _BufferID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}
	VertexBuffer::~VertexBuffer() {
		IMAGINE_PROFILE_FUNCTION();

		glDeleteBuffers(1, &_BufferID);
	}

	void VertexBuffer::Bind() const{
		IMAGINE_PROFILE_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, _BufferID);
	}
	
	void VertexBuffer::Unbind() const {
		IMAGINE_PROFILE_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	
	void VertexBuffer::SetData(const void* data, size_t size) {
		glBindBuffer(GL_ARRAY_BUFFER, _BufferID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}
}
#endif