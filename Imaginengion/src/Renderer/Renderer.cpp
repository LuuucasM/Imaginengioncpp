#include "impch.h"
#include "Renderer.h"


namespace IM {

	Renderer::SceneData* Renderer::_SceneData = new Renderer::SceneData();

	void Renderer::BeginScene(OrthographicCamera& camera) {
		_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}
	void Renderer::EndScene() {

	}

	void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, C_Transform transform) {
		shader->Bind();
		shader->SetUniform("u_ViewProjection", _SceneData->ViewProjectionMatrix);
		shader->SetUniform("u_Transform", transform.Transform);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}