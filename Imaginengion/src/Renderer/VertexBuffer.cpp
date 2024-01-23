#include "impch.h"

#include "VertexBuffer.h"
#include "Platform/OpenGL/OpenGLVertexBuffer.h"

#include "RendererAPI.h"

namespace IM {
	RefPtr<VertexBuffer> VertexBuffer::Create(size_t size)
	{
		switch (RendererAPI::GetCurrentAPI()) {
		case RendererAPI::API::None:
			IMAGINE_CORE_ASSERT(false, "RendererAPI::API::None is currently not supported !");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRefPtr<OpenGLVertexBuffer>(size);
		}
		IMAGINE_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
	RefPtr<VertexBuffer> VertexBuffer::Create(float* vertices, size_t size) {
		switch (RendererAPI::GetCurrentAPI()) {
			case RendererAPI::API::None:
				IMAGINE_CORE_ASSERT(false, "RendererAPI::API::None is currently not supported !");
				return nullptr;
			case RendererAPI::API::OpenGL:
				return CreateRefPtr<OpenGLVertexBuffer>(vertices, size);
		}
		IMAGINE_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}