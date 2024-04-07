#pragma once

#include "RendererAPI.h"
#include "RenderCommand.h"
#include "OrthographicCamera.h"
#include "Shader.h"
#include "Texture.h"
#include "Subtexture2D.h"
#include "ECS/Components.h"
#include "EditorCamera.h"

namespace IM {
	class Renderer
	{
	public:
		static void Init();
		static void Shutdown();
		static void OnWindowResize(size_t width, size_t height);
		static void SetClearColor(const glm::vec4& color) { RenderCommand::SetClearColor(color); }
		static void Clear() { RenderCommand::Clear(); }

		//==========3D==========3D==========3D==========3D==========3D==========3D==========3D==========3D==========3D==========3D
		class R3D
		{
		public:
			static void BeginScene(OrthographicCamera& camera);
			static void EndScene();

			static void Submit(const RefPtr<Shader>& shader, const RefPtr<VertexArray>& vertexArray, C_Transform transform);

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
			static void BeginScene(const OrthographicCamera& camera); //TODO: Remove
			static void BeginScene(const C_Camera camera, const C_Transform transform);
			static void BeginScene(EditorCamera& camera);
			static void EndScene();

			static void FlushScene();

			static void DrawRect(const glm::vec2& position, const glm::vec2& scale, const glm::vec4& color);
			static void DrawRect(const glm::vec3& position, const glm::vec2& scale, const glm::vec4& color);
			static void DrawRect(const glm::vec2& position, const glm::vec2& scale, const RefPtr<Texture2D> texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
			static void DrawRect(const glm::vec3& position, const glm::vec2& scale, const RefPtr<Texture2D> texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
			static void DrawRect(const glm::vec2& position, const glm::vec2& scale, const RefPtr<SubTexture2D> subtexture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
			static void DrawRect(const glm::vec3& position, const glm::vec2& scale, const RefPtr<SubTexture2D> subtexture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));

			static void DrawRect(const glm::mat4& transform, const glm::vec4& color, uint32_t entityID = 0);
			static void DrawRect(const glm::mat4& transform, const RefPtr<Texture2D> texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f), uint32_t entityID = 0);

			static void DrawRotatedRect(const glm::vec2& position, const glm::vec2& scale, float rotation, const glm::vec4& color);
			static void DrawRotatedRect(const glm::vec3& position, const glm::vec2& scale, float rotation, const glm::vec4& color);
			static void DrawRotatedRect(const glm::vec2& position, const glm::vec2& scale, float rotation, const RefPtr<Texture2D> texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
			static void DrawRotatedRect(const glm::vec3& position, const glm::vec2& scale, float rotation, const RefPtr<Texture2D> texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
			static void DrawRotatedRect(const glm::vec2& position, const glm::vec2& scale, float rotation, const RefPtr<SubTexture2D> subtexture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
			static void DrawRotatedRect(const glm::vec3& position, const glm::vec2& scale, float rotation, const RefPtr<SubTexture2D> subtexture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));

			static void DrawRotatedRect(const glm::mat4& transform, const glm::vec4& color);
			static void DrawRotatedRect(const glm::mat4& transform, const RefPtr<Texture2D> texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));

			static void DrawSprite(const glm::mat4& transform, const C_SpriteRenderer& src, uint32_t entityID);
			static void DrawCircle(const glm::mat4& transform, const glm::vec4& color, float thickness = 1.0f, float fade = 0.005f, uint32_t entityID = 0);
			
			static void DrawEditorLine(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4& color, float thickness = 1.0f);
			static void DrawEditorRect(const glm::vec3& position, const glm::vec2& scale, const glm::vec4& color);
			static void DrawEditorRect(const glm::mat4& transform, const glm::vec4& color);

			static const float GetEditorLineThickness();
			static void SetEditorLineThickness(float thickness);

			struct Statistics {
				uint32_t DrawCalls = 0;
				uint32_t RectCount = 0;

				uint32_t GetTotalVertexCount() const { return RectCount * 4; }
				uint32_t GetTotalIndexCount() const { return RectCount * 6; }
			};
			static Statistics GetStats();
			static void ResetStats();

		private:

			static void StartBatch();
			static void NextBatch();

			static void Init();
			static void Shutdown();
			friend class Renderer;
		};
		//==========2D==========2D==========2D==========2D==========2D==========2D==========2D==========2D==========2D==========2D
	};
}