#include "impch.h"
#include "FrameBuffer.h"

#include "Renderer/RendererAPI.h"
#include "Platform/OpenGL/OpenGLFrameBuffer.h"

namespace IM {
	RefPtr<FrameBuffer> FrameBuffer::Create(const FrameBufferSpecification& spec)
	{
		switch (RendererAPI::GetCurrentAPI()) {
		case RendererAPI::API::None:
			IMAGINE_CORE_ASSERT(false, "RendererAPI::API::None is currently not supported in FrameBuffer::Create!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRefPtr<OpenGLFrameBuffer>(spec);
		}
		IMAGINE_CORE_ASSERT(false, "Unknown RendererAPI in FrameBuffer::Create!");
		return nullptr;
	}
}