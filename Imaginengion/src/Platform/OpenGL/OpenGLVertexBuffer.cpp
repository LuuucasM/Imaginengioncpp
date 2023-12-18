#include "impch.h"

#include "OpenGLVertexBuffer.h"

#include "glad/glad.h"

namespace IM {
	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size) {

		IMAGINE_PROFILE_FUNCTION();

		glCreateBuffers(1, &_BufferID);
		glBindBuffer(GL_ARRAY_BUFFER, _BufferID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}
	OpenGLVertexBuffer::~OpenGLVertexBuffer() {

		IMAGINE_PROFILE_FUNCTION();

		glDeleteBuffers(1, &_BufferID);
	}
	void OpenGLVertexBuffer::Bind() const {

		IMAGINE_PROFILE_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, _BufferID);
	}
	void OpenGLVertexBuffer::Unbind() const {

		IMAGINE_PROFILE_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}