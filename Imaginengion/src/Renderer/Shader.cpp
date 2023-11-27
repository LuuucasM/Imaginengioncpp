#include "impch.h"
#include "Shader.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace IM {
	Shader* Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrc) {
		switch (RendererAPI::GetCurrentAPI()) {
		case RendererAPI::API::None:
			IMAGINE_CORE_ASSERT(false, "RendererAPI::API::None is currently not supported !");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return new OpenGLShader(vertexSrc, fragmentSrc);
		}
		IMAGINE_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}