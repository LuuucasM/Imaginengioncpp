#pragma once

#include "RenderCommand.h"

#include "OrthographicCamera.h"
#include "Shader.h"
#include "ECS/Components.h"

namespace IM {
	class Renderer
	{
	public:
		static void Init();

		static void OnWindowResize(int width, int height);

		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();

		static void Submit(const RefPtr<Shader>& shader, const RefPtr<VertexArray>& vertexArray, C_Transform transform = C_Transform());

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetCurrentAPI(); }
	private:
		struct SceneData {
			glm::mat4 ViewProjectionMatrix;
		};

		static SceneData* _SceneData;
	};
}



