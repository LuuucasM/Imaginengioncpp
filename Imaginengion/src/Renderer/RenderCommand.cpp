#include "impch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace IM {
	RendererAPI* RenderCommand::_RendererAPI = new OpenGLRendererAPI();
}