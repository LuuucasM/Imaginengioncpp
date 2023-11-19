#include "impch.h"

#include "VertexBuffer.h"
#include "Platform/OpenGL/OpenGLVertexBuffer.h"

#include "RendererAPI.h"

namespace IM {
	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size) {
		switch (RendererAPI::GetCurrentAPI()) {
			case RendererAPI::API::None:
				IMAGINE_CORE_ASSERT(false, "RendererAPI::API::None is currently not supported !");
				return nullptr;
			case RendererAPI::API::OpenGL:
				return new OpenGLVertexBuffer(vertices, size);
		}
		IMAGINE_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}