#include "impch.h"

#include "RendererAPI.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace IM {
	RendererAPI::API RendererAPI::_API = RendererAPI::API::OpenGL;


	ScopePtr<RendererAPI> RendererAPI::Create()
	{
		switch (_API) {
		case RendererAPI::API::None:
			IMAGINE_CORE_ASSERT(0, "RendererAPI::None is currently not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateScopePtr<OpenGLRendererAPI>();
		}
		IMAGINE_CORE_ASSERT(0, "Unkown RendererAPI in RendererAPI::Create!");
		return nullptr;
	}

}