#include "impch.h"

#include "OpenGLVertexBuffer.h"

#include "glad/glad.h"

namespace IM {
	OpenGLVertexBuffer::OpenGLVertexBuffer(size_t size)
	{
		IMAGINE_PROFILE_FUNCTION();

		glCreateBuffers(1, &_BufferID);
		glBindBuffer(GL_ARRAY_BUFFER, _BufferID);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}
	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, size_t size) {

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
	void OpenGLVertexBuffer::SetData(const void* data, size_t size)
	{
		glBindBuffer(GL_ARRAY_BUFFER, _BufferID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}
}