#include "impch.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace IM {

	Renderer::SceneData* Renderer::_SceneData = new Renderer::SceneData();

	void Renderer::Init() {
		RenderCommand::Init();
	}

	void Renderer::BeginScene(OrthographicCamera& camera) {
		_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}
	void Renderer::EndScene() {

	}

	void Renderer::Submit(const RefPtr<Shader>& shader, const RefPtr<VertexArray>& vertexArray, C_Transform transform) {
		shader->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->SetUniform("u_ViewProjection", _SceneData->ViewProjectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->SetUniform("u_Transform", transform.Transform);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}