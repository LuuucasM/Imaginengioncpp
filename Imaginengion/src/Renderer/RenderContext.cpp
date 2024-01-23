#include "impch.h"

#include "RenderContext.h"

#include "Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLContext.h"

namespace IM {
	ScopePtr<RenderContext> RenderContext::Create(void* window) {
		switch (RendererAPI::GetCurrentAPI()) {
			case RendererAPI::API::None: 
				IMAGINE_CORE_ASSERT(0, "RendererAPI::None is currently not supported!");
				return nullptr;
			case RendererAPI::API::OpenGL:
				return CreateScopePtr<OpenGLContext>(static_cast<GLFWwindow*>(window));
		}
		IMAGINE_CORE_ASSERT(0, "Unknown RendererAPI in RenderContext::Create!");
		return nullptr;
	}
}