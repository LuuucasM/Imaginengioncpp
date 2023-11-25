#pragma once

#include "RenderCommand.h"

#include "OrthographicCamera.h"
#include "Shader.h"
#include "ECS/Components.h"

namespace IM {
	class Renderer
	{
	public:
		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();

		static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, C_Transform transform = C_Transform());

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetCurrentAPI(); }
	private:
		struct SceneData {
			glm::mat4 ViewProjectionMatrix;
		};

		static SceneData* _SceneData;
	};
}



