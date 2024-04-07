#include "impch.h"

#ifdef IMAGINE_OPENGL
#include "Renderer/RendererAPI.h"

#include "glad/glad.h"

namespace IM {
	namespace {
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
	}
	void RendererAPI::Init() {
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

	void RendererAPI::SetViewport(int x, int y, size_t width, size_t height) {
		glViewport(x, y, (GLsizei)width, (GLsizei)height);
	}

	void RendererAPI::SetClearColor(const glm::vec4& color) {
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void RendererAPI::Clear() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void RendererAPI::DrawIndexed(const RefPtr<VertexArray>& vertexArray, uint32_t indexCount) {
		vertexArray->Bind();
		uint32_t count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount();
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
	}

	void RendererAPI::DrawLines(const RefPtr<VertexArray>& vertexArray, uint32_t vertexCount) {
		vertexArray->Bind();
		glDrawArrays(GL_LINES, 0, vertexCount);
	}

	void RendererAPI::SetEditorLineThickness(float thickness) {
		glLineWidth(thickness);
	}
}
#endif