#include "impch.h"
#include "RenderCommand.h"

namespace IM {
	ScopePtr<RendererAPI> RenderCommand::_RendererAPI = RendererAPI::Create();
}