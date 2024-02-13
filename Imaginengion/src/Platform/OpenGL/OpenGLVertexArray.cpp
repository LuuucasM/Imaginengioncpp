#include "impch.h"
#include "OpenGLVertexArray.h"

#include "glad/glad.h"

namespace IM {

	static GLenum ShaderDataTypeToOpneGLBaseType(ShaderDataType type) {
		switch (type)
		{
		case ShaderDataType::Float:    return GL_FLOAT;
		case ShaderDataType::Float2:   return GL_FLOAT;
		case ShaderDataType::Float3:   return GL_FLOAT;
		case ShaderDataType::Float4:   return GL_FLOAT;
		case ShaderDataType::Mat3:     return GL_FLOAT;
		case ShaderDataType::Mat4:     return GL_FLOAT;
		case ShaderDataType::Int:      return GL_INT;
		case ShaderDataType::Int2:     return GL_INT;
		case ShaderDataType::Int3:     return GL_INT;
		case ShaderDataType::Int4:     return GL_INT;
		case ShaderDataType::Bool:     return GL_BOOL;
		}
		IMAGINE_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray() {

		IMAGINE_PROFILE_FUNCTION();

		glCreateVertexArrays(1, &_ArrayID);
	}
	OpenGLVertexArray::~OpenGLVertexArray() {

		IMAGINE_PROFILE_FUNCTION();

		glDeleteVertexArrays(1, &_ArrayID);
	}
	void OpenGLVertexArray::Bind() const {

		IMAGINE_PROFILE_FUNCTION();

		glBindVertexArray(_ArrayID);
	}
	void OpenGLVertexArray::Unbind() const {

		IMAGINE_PROFILE_FUNCTION();

		glBindVertexArray(0);
	}
	void OpenGLVertexArray::AddVertexBuffer(const RefPtr<VertexBuffer> vertexBuffer) {

		IMAGINE_PROFILE_FUNCTION();

		IMAGINE_CORE_ASSERT(vertexBuffer->GetLayout().size(), "Vertex Buffer has no layout!");

		glBindVertexArray(_ArrayID);
		vertexBuffer->Bind();

		uint32_t index = 0;
		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout) {
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, element.GetComponentCount(),
				ShaderDataTypeToOpneGLBaseType(element.Type),
				element.bNormalized ? GL_TRUE : GL_FALSE,
				vertexBuffer->GetStride(),
				(const void*)(intptr_t)element.Offset);
			++index;
		}

		_VertexBuffers.push_back(vertexBuffer);
	}
	void OpenGLVertexArray::SetIndexBuffer(const RefPtr<IndexBuffer> indexBuffer) {

		IMAGINE_PROFILE_FUNCTION();

		glBindVertexArray(_ArrayID);
		indexBuffer->Bind();

		_IndexBuffer = indexBuffer;
	}
}