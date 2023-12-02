#include "impch.h"

#include "IndexBuffer.h"
#include "Platform/OpenGL/OpenGLIndexBuffer.h"

#include "RendererAPI.h"

namespace IM {
	RefPtr<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t size) {
		switch (RendererAPI::GetCurrentAPI()) {
		case RendererAPI::API::None:
			IMAGINE_CORE_ASSERT(false, "RendererAPI::API::None is currently not supported !");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRefPtr<OpenGLIndexBuffer>(indices, size);
		}
		IMAGINE_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}