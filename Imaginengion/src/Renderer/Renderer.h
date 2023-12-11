#pragma once

#include "RendererAPI.h"
#include "RenderCommand.h"
#include "OrthographicCamera.h"
#include "Shader.h"
#include "Texture.h"
#include "ECS/Components.h"

namespace IM {
	class Renderer
	{
	public:
		static void Init();
		static void Shutdown();
		static void OnWindowResize(int width, int height);
		inline static void SetClearColor(const glm::vec4& color) { RenderCommand::SetClearColor(color); }
		inline static void Clear() { RenderCommand::Clear(); }
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

			static ScopePtr<SceneData> _SceneData;
			friend class Renderer;
		};
		//==========3D==========3D==========3D==========3D==========3D==========3D==========3D==========3D==========3D==========3D


		//==========2D==========2D==========2D==========2D==========2D==========2D==========2D==========2D==========2D==========2D
		class R2D
		{
		public:
			static void BeginScene(const OrthographicCamera& camera);
			static void EndScene();

			static void DrawRect(const glm::vec2& position, const glm::vec2& scale, const glm::vec4& color);
			static void DrawRect(const glm::vec3& position, const glm::vec2& scale, const glm::vec4& color);
			static void DrawRect(const glm::vec2& position, const glm::vec2& scale, const RefPtr<Texture2D> texture);
			static void DrawRect(const glm::vec3& position, const glm::vec2& scale, const RefPtr<Texture2D> texutre);
		private:
			static void Init();
			static void Shutdown();
			friend class Renderer;
		};
		//==========2D==========2D==========2D==========2D==========2D==========2D==========2D==========2D==========2D==========2D
	};
}