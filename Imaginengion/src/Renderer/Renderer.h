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
		static void Shutdown();
		static void OnWindowResize(int width, int height);

		//==========3D==========3D==========3D==========3D==========3D==========3D==========3D==========3D==========3D==========3D
		class R3D
		{
		public:
			static void BeginScene(OrthographicCamera& camera);
			static void EndScene();

			static void Submit(const RefPtr<Shader>& shader, const RefPtr<VertexArray>& vertexArray, C_Transform transform = C_Transform());

			inline static RendererAPI::API GetAPI() { return RendererAPI::GetCurrentAPI(); }
		private:
			static void Init();
			static void Shutdown();
		private:
			struct SceneData {
				glm::mat4 ViewProjectionMatrix;
			};

			static SceneData* _SceneData;
			friend class Renderer;
		};
		//==========3D==========3D==========3D==========3D==========3D==========3D==========3D==========3D==========3D==========3D


		//==========2D==========2D==========2D==========2D==========2D==========2D==========2D==========2D==========2D==========2D
		class R2D
		{
		public:
			static void BeginScene(const OrthographicCamera& camera);
			static void EndScene();

			static void DrawRect(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
			static void DrawRect(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		private:
			static void Init();
			static void Shutdown();
			friend class Renderer;
		};
		//==========2D==========2D==========2D==========2D==========2D==========2D==========2D==========2D==========2D==========2D
	};
}