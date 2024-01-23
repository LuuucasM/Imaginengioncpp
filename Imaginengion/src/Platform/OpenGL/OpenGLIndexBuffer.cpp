#include "impch.h"

#include "OpenGLIndexBuffer.h"

#include "glad/glad.h"

namespace IM {
	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count)
	{
		_Count = count;
		glCreateBuffers(1, &_BufferID);

		// GL_ELEMENT_ARRAY_BUFFER is not valid without an actively bound VAO
		// Binding with GL_ARRAY_BUFFER allows the data to be loaded regardless of VAO state. 
		glBindBuffer(GL_ARRAY_BUFFER, _BufferID);
		glBufferData(GL_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}
	OpenGLIndexBuffer::~OpenGLIndexBuffer() {
		glDeleteBuffers(1, &_BufferID);
	}
	void OpenGLIndexBuffer::Bind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _BufferID);
	}
	void OpenGLIndexBuffer::Unbind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}