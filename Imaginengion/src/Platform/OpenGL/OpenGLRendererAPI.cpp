#include "impch.h"
#include "OpenGLRendererAPI.h"

#include "glad/glad.h"

namespace IM {

	void OpenGLMessageCallback(
		unsigned souce,
		unsigned type,
		unsigned id,
		unsigned severity,
		int length,
		const char* message,
		const void* userParam) {
		switch (severity) {
			case GL_DEBUG_SEVERITY_HIGH:
				IMAGINE_CORE_CRITICAL(message);
				return;
			case GL_DEBUG_SEVERITY_MEDIUM:
				IMAGINE_CORE_ERROR(message);
				return;
			case GL_DEBUG_SEVERITY_LOW:
				IMAGINE_CORE_WARN(message);
				return;
			case GL_DEBUG_SEVERITY_NOTIFICATION:
				IMAGINE_CORE_INFO(message);

		}
		IMAGINE_CORE_ASSERT(0, "Unknown severity level!");
	}

	void OpenGLRendererAPI::Init() {

		IMAGINE_PROFILE_FUNCTION();

		#ifdef IMAGINE_DEBUG
			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			glDebugMessageCallback(OpenGLMessageCallback, nullptr);
			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
		#endif

		glEnable(GL_TEXTURE_2D);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
	}

	void OpenGLRendererAPI::SetViewport(int x, int y, size_t width, size_t height)
	{
		glViewport(x, y, width, height);
	}

	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color) {
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::Clear() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::DrawIndexed(const RefPtr<VertexArray>& vertexArray, uint32_t indexCount) {

		uint32_t count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount();
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}