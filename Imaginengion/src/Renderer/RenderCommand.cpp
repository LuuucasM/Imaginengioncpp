#include "impch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace IM {
	ScopePtr<RendererAPI> RenderCommand::_RendererAPI = RendererAPI::Create();
}