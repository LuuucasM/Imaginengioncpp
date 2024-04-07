#include "impch.h"

#include "Renderer.h"
#include "UniformBuffer.h"

#include <glm/gtc/matrix_transform.hpp>

namespace IM {
	ScopePtr<Renderer::R3D::SceneData> Renderer::R3D::_SceneData = CreateScopePtr<Renderer::R3D::SceneData>();

	void Renderer::Init() {
		IMAGINE_PROFILE_FUNCTION();
		RenderCommand::Init();
		Renderer::R3D::Init();
		Renderer::R2D::Init();
	}

	void Renderer::Shutdown() {
		Renderer::R3D::Shutdown();
		Renderer::R2D::Shutdown();
	}

	void Renderer::OnWindowResize(size_t width, size_t height){
		RenderCommand::SetViewport(0, 0, width, height);
	}
	//==========3D==========3D==========3D==========3D==========3D==========3D==========3D==========3D==========3D==========3D
	void Renderer::R3D::Init() {

	}
	void Renderer::R3D::Shutdown() {

	}
	void Renderer::R3D::BeginScene(OrthographicCamera& camera) {
		_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}
	void Renderer::R3D::EndScene() {

	}

	void Renderer::R3D::Submit(const RefPtr<Shader>& shader, const RefPtr<VertexArray>& vertexArray, C_Transform transform) {
		shader->Bind();
		shader->SetValue("u_ViewProjection", _SceneData->ViewProjectionMatrix);
		shader->SetValue("u_Transform", transform.GetTransform());

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
	//==========3D==========3D==========3D==========3D==========3D==========3D==========3D==========3D==========3D==========3D


	//==========2D==========2D==========2D==========2D==========2D==========2D==========2D==========2D==========2D==========2D

	struct RectVertex {
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float TexIndex;
		float TilingFactor;
		// TODO: maskid
		//Editor only
		uint32_t EntityID;
	};

	struct CircleVertex {
		glm::vec3 Position;
		glm::vec3 LocalPosition;
		glm::vec4 Color;
		float Thickness;
		float Fade;
		// TODO: maskid
		//Editor only
		uint32_t EntityID;
	};

	struct EditorLineVertex { //to be used for editor and debug not production
		glm::vec3 Position;
		glm::vec4 Color;
	};

	struct Renderer2DData {

		static const uint32_t MaxRect = 10000; //TODO: MAKE CLASS RENDERER CAPABILITIES
		static const uint32_t MaxVerticies = MaxRect * 4;
		static const uint32_t MaxIndices = MaxRect * 6;
		static const uint32_t MaxTextureSlots = 32; //TODO: MAKE CLASS RENDERER CAPABILITIES

		RefPtr<VertexArray> _SpriteVertexArray;
		RefPtr<VertexBuffer> _SpriteVertexBuffer;
		RefPtr<Shader> _SpriteShader;

		RefPtr<VertexArray> _CircleVertexArray;
		RefPtr<VertexBuffer> _CircleVertexBuffer;
		RefPtr<Shader> _CircleShader;

		RefPtr<VertexArray> _EditorLineVertexArray;
		RefPtr<VertexBuffer> _EditorLineVertexBuffer;
		RefPtr<Shader> _EditorLineShader;

		RefPtr<Texture2D> _WhiteTexture;

		uint32_t RectIndexCount = 0;
		RectVertex* RectVertexBufferBase = nullptr;
		RectVertex* RectVertexBufferPtr = nullptr;

		uint32_t CircleIndexCount = 0;
		CircleVertex* CircleVertexBufferBase = nullptr;
		CircleVertex* CircleVertexBufferPtr = nullptr;

		uint32_t EditorLineVertexCount = 0;
		EditorLineVertex* EditorLineVertexBufferBase = nullptr;
		EditorLineVertex* EditorLineVertexBufferPtr = nullptr;

		float EditorLineThickness = 2.0f;

		std::array<RefPtr<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1; // texture slot 0 == white texture

		glm::vec4 RectVertexPositions[4];
		glm::vec4 CircleVertexPositions[4];

		Renderer::R2D::Statistics Stats;

		struct CameraData {
			glm::mat4 ViewProjection;
		};
		CameraData CameraBuffer;
		RefPtr<UniformBuffer> CameraUniformBuffer;
	};

	static Renderer2DData _Data;

	void Renderer::R2D::Init() {

		IMAGINE_PROFILE_FUNCTION();

		_Data._SpriteVertexArray = VertexArray::Create();
		_Data._CircleVertexArray = VertexArray::Create();
		_Data._EditorLineVertexArray = VertexArray::Create();

		_Data._SpriteShader = Shader::Create("assets/shaders/Render2D_Sprite.glsl");
		_Data._CircleShader = Shader::Create("assets/shaders/Render2D_Circle.glsl");
		_Data._EditorLineShader = Shader::Create("assets/shaders/Render2D_EditorLine.glsl");

		int samplers[_Data.MaxTextureSlots];
		for (uint32_t i = 0; i < _Data.MaxTextureSlots; i++) {
			samplers[i] = i;
		}

		uint32_t* rectIndices = new uint32_t[_Data.MaxIndices];
		uint32_t offset = 0;
		for (uint32_t i = 0; i < _Data.MaxIndices; i += 6) {
			rectIndices[i + 0] = offset + 0;
			rectIndices[i + 1] = offset + 1;
			rectIndices[i + 2] = offset + 2;

			rectIndices[i + 3] = offset + 2;
			rectIndices[i + 4] = offset + 3;
			rectIndices[i + 5] = offset + 0;

			offset += 4;
		}
		RefPtr<IndexBuffer> rectIB = IndexBuffer::Create(rectIndices, _Data.MaxIndices);

		//sprite stuff
		_Data._SpriteVertexBuffer = VertexBuffer::Create(_Data.MaxVerticies * sizeof(RectVertex));
		_Data._SpriteVertexBuffer->SetLayout(_Data._SpriteShader->GetLayout());
		_Data._SpriteVertexBuffer->SetStride(_Data._SpriteShader->GetStride());

		_Data._SpriteVertexArray->AddVertexBuffer(_Data._SpriteVertexBuffer);
		
		_Data._SpriteVertexArray->SetIndexBuffer(rectIB);

		_Data.RectVertexBufferBase = new RectVertex[_Data.MaxVerticies];

		//circle stuff
		_Data._CircleVertexBuffer = VertexBuffer::Create(_Data.MaxVerticies * sizeof(CircleVertex));
		_Data._CircleVertexBuffer->SetStride(_Data._CircleShader->GetStride());
		_Data._CircleVertexBuffer->SetLayout(_Data._CircleShader->GetLayout());

		_Data._CircleVertexArray->AddVertexBuffer(_Data._CircleVertexBuffer);

		_Data._CircleVertexArray->SetIndexBuffer(rectIB); //use rectIB

		_Data.CircleVertexBufferBase = new CircleVertex[_Data.MaxVerticies];


		_Data._EditorLineVertexBuffer = VertexBuffer::Create(_Data.MaxVerticies * sizeof(EditorLineVertex));
		_Data._EditorLineVertexBuffer->SetStride(_Data._EditorLineShader->GetStride());
		_Data._EditorLineVertexBuffer->SetLayout(_Data._EditorLineShader->GetLayout());

		_Data._EditorLineVertexArray->AddVertexBuffer(_Data._EditorLineVertexBuffer);

		_Data._EditorLineVertexArray->SetIndexBuffer(rectIB); //use rectIB

		_Data.EditorLineVertexBufferBase = new EditorLineVertex[_Data.MaxVerticies];

		delete[] rectIndices;

		//white texture
		_Data._WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0;
		whiteTextureData = ~whiteTextureData;
		_Data._WhiteTexture->SetData(&whiteTextureData, sizeof(whiteTextureData));

		_Data.TextureSlots[0] = _Data._WhiteTexture;

		_Data.RectVertexPositions[0] = {-0.5f, -0.5f, 0.0f, 1.0f};
		_Data.RectVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		_Data.RectVertexPositions[2] = { 0.5f, 0.5f, 0.0f, 1.0f };
		_Data.RectVertexPositions[3] = { -0.5f, 0.5f, 0.0f, 1.0f };

		_Data.CameraUniformBuffer = UniformBuffer::Create(sizeof(Renderer2DData::CameraData), 0);
	}

	void Renderer::R2D::Shutdown() {
		IMAGINE_PROFILE_FUNCTION();
	}

	void Renderer::R2D::BeginScene(const OrthographicCamera& camera)
	{
		IMAGINE_PROFILE_FUNCTION();

		_Data._SpriteShader->Bind();
		_Data._SpriteShader->SetValue("u_ViewProjection", camera.GetViewProjectionMatrix());

		StartBatch();

	}
	void Renderer::R2D::BeginScene(const C_Camera camera, C_Transform transform)
	{
		IMAGINE_PROFILE_FUNCTION();

		_Data.CameraBuffer.ViewProjection = camera.Projection * glm::inverse(transform.GetTransform());
		_Data.CameraUniformBuffer->SetData(&_Data.CameraBuffer, sizeof(Renderer2DData::CameraData));

		StartBatch();
	}
	void Renderer::R2D::BeginScene(EditorCamera& camera)
	{
		IMAGINE_PROFILE_FUNCTION();

		_Data.CameraBuffer.ViewProjection = camera.GetViewProjection();
		_Data.CameraUniformBuffer->SetData(&_Data.CameraBuffer, sizeof(Renderer2DData::CameraData));

		StartBatch();
	}
	void Renderer::R2D::EndScene()
	{
		IMAGINE_PROFILE_FUNCTION();

		__int64 dataSize = (uint8_t*)_Data.RectVertexBufferPtr - (uint8_t*)_Data.RectVertexBufferBase;
		_Data._SpriteVertexBuffer->SetData(_Data.RectVertexBufferBase, dataSize);

		FlushScene();
	}

	void Renderer::R2D::StartBatch() {

		_Data.RectIndexCount = 0;
		_Data.RectVertexBufferPtr = _Data.RectVertexBufferBase;

		_Data.CircleIndexCount = 0;
		_Data.CircleVertexBufferPtr = _Data.CircleVertexBufferBase;

		_Data.EditorLineVertexCount = 0;
		_Data.EditorLineVertexBufferPtr = _Data.EditorLineVertexBufferBase;

		_Data.TextureSlotIndex = 1;
	}
	void Renderer::R2D::NextBatch()
	{
		FlushScene();
		StartBatch();
	}
	void Renderer::R2D::FlushScene()
	{
		if (_Data.RectIndexCount) {
			uint32_t dataSize = (uint32_t)((uint8_t*)_Data.RectVertexBufferPtr - (uint8_t*)_Data.RectVertexBufferBase);
			_Data._SpriteVertexBuffer->SetData(_Data.RectVertexBufferBase, dataSize);

			//Bind Textures;
			for (uint32_t i = 0; i < _Data.TextureSlotIndex; i++) {
				_Data.TextureSlots[i]->Bind(i);
			}

			_Data._SpriteShader->Bind();
			RenderCommand::DrawIndexed(_Data._SpriteVertexArray, _Data.RectIndexCount);
			++_Data.Stats.DrawCalls;
		}

		if (_Data.CircleIndexCount) {
			uint32_t dataSize = (uint32_t)((uint8_t*)_Data.CircleVertexBufferPtr - (uint8_t*)_Data.CircleVertexBufferBase);
			_Data._CircleVertexBuffer->SetData(_Data.CircleVertexBufferBase, dataSize);

			_Data._CircleShader->Bind();
			RenderCommand::DrawIndexed(_Data._CircleVertexArray, _Data.CircleIndexCount);
			++_Data.Stats.DrawCalls;
		}

		if (_Data.EditorLineVertexCount) {
			uint32_t dataSize = (uint32_t)((uint8_t*)_Data.EditorLineVertexBufferPtr - (uint8_t*)_Data.EditorLineVertexBufferBase);
			_Data._EditorLineVertexBuffer->SetData(_Data.EditorLineVertexBufferBase, dataSize);

			_Data._EditorLineShader->Bind();
			RenderCommand::SetEditorLineThickness(_Data.EditorLineThickness);
			RenderCommand::DrawLines(_Data._EditorLineVertexArray, _Data.EditorLineVertexCount);
			++_Data.Stats.DrawCalls;
		}
	}

	void Renderer::R2D::DrawRect(const glm::vec2& position, const glm::vec2& scale, const glm::vec4& color)
	{
		DrawRect({ position.x, position.y, 0.0f }, scale, color);
	}
	void Renderer::R2D::DrawRect(const glm::vec3& position, const glm::vec2& scale, const glm::vec4& color)
	{

		IMAGINE_PROFILE_FUNCTION();

		if (_Data.RectIndexCount + 6 > Renderer2DData::MaxIndices) {
			NextBatch();
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { scale.x, scale.y, 1.0f });

		DrawRect(transform, color);
	}
	void Renderer::R2D::DrawRect(const glm::vec2& position, const glm::vec2& scale, const RefPtr<Texture2D> texture, float tilingFactor, const glm::vec4& tintColor)
	{
		DrawRect({ position.x, position.y, 0.0f }, scale, texture, tilingFactor, tintColor);
	}
	void Renderer::R2D::DrawRect(const glm::vec3& position, const glm::vec2& scale, const RefPtr<Texture2D> texture, float tilingFactor, const glm::vec4& tintColor)
	{

		IMAGINE_PROFILE_FUNCTION();

		constexpr size_t rectVertexCount = 4;
		constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

		if (_Data.RectIndexCount + 6 > Renderer2DData::MaxIndices) {
			NextBatch();
		}

		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < _Data.TextureSlotIndex; i++) {
			if (*_Data.TextureSlots[i].get() == *texture.get()) {
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f) {
			textureIndex = (float)_Data.TextureSlotIndex;
			_Data.TextureSlots[_Data.TextureSlotIndex] = texture;
			_Data.TextureSlotIndex++;
		}
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { scale.x, scale.y, 1.0f });

		DrawRect(transform, texture, tilingFactor, tintColor);
	}

	void Renderer::R2D::DrawRect(const glm::vec2& position, const glm::vec2& scale, const RefPtr<SubTexture2D> subtexture, float tilingFactor, const glm::vec4& tintColor)
	{
		DrawRect({ position.x, position.y, 0.0f }, scale, subtexture, tilingFactor, tintColor);
	}
	void Renderer::R2D::DrawRect(const glm::vec3& position, const glm::vec2& scale, const RefPtr<SubTexture2D> subtexture, float tilingFactor, const glm::vec4& tintColor)
	{

		IMAGINE_PROFILE_FUNCTION();

		constexpr size_t rectVertexCount = 4;
		constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

		const glm::vec2* textureCoords = subtexture->GetTexCoords();
		const RefPtr<Texture2D> texture = subtexture->GetTexture();

		if (_Data.RectIndexCount + 6 > Renderer2DData::MaxIndices) {
			NextBatch();
		}

		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < _Data.TextureSlotIndex; i++) {
			if (*_Data.TextureSlots[i].get() == *texture.get()) {
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f) {
			textureIndex = (float)_Data.TextureSlotIndex;
			_Data.TextureSlots[_Data.TextureSlotIndex] = texture;
			_Data.TextureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { scale.x, scale.y, 1.0f });

		_Data.RectVertexBufferPtr->Position = transform * _Data.RectVertexPositions[0];
		_Data.RectVertexBufferPtr->Color = color;
		_Data.RectVertexBufferPtr->TexCoord = textureCoords[0];
		_Data.RectVertexBufferPtr->TexIndex = textureIndex;
		_Data.RectVertexBufferPtr->TilingFactor = tilingFactor;
		_Data.RectVertexBufferPtr++;

		_Data.RectVertexBufferPtr->Position = transform * _Data.RectVertexPositions[1];
		_Data.RectVertexBufferPtr->Color = color;
		_Data.RectVertexBufferPtr->TexCoord = textureCoords[1];
		_Data.RectVertexBufferPtr->TexIndex = textureIndex;
		_Data.RectVertexBufferPtr->TilingFactor = tilingFactor;
		_Data.RectVertexBufferPtr++;

		_Data.RectVertexBufferPtr->Position = transform * _Data.RectVertexPositions[2];
		_Data.RectVertexBufferPtr->Color = color;
		_Data.RectVertexBufferPtr->TexCoord = textureCoords[2];
		_Data.RectVertexBufferPtr->TexIndex = textureIndex;
		_Data.RectVertexBufferPtr->TilingFactor = tilingFactor;
		_Data.RectVertexBufferPtr++;

		_Data.RectVertexBufferPtr->Position = transform * _Data.RectVertexPositions[3];
		_Data.RectVertexBufferPtr->Color = color;
		_Data.RectVertexBufferPtr->TexCoord = textureCoords[3];
		_Data.RectVertexBufferPtr->TexIndex = textureIndex;
		_Data.RectVertexBufferPtr->TilingFactor = tilingFactor;
		_Data.RectVertexBufferPtr++;

		_Data.RectIndexCount += 6;

		_Data.Stats.RectCount++;
	}

	void Renderer::R2D::DrawRect(const glm::mat4& transform, const glm::vec4& color, uint32_t entityID)
	{
		IMAGINE_PROFILE_FUNCTION();

		if (_Data.RectIndexCount + 6 > Renderer2DData::MaxIndices) {
			NextBatch();
		}

		_Data.RectVertexBufferPtr->Position = transform * _Data.RectVertexPositions[0];
		_Data.RectVertexBufferPtr->Color = color;
		_Data.RectVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
		_Data.RectVertexBufferPtr->TexIndex = 0.0f;
		_Data.RectVertexBufferPtr->TilingFactor = 1.0f;
		_Data.RectVertexBufferPtr->EntityID = entityID;
		_Data.RectVertexBufferPtr++;

		_Data.RectVertexBufferPtr->Position = transform * _Data.RectVertexPositions[1];
		_Data.RectVertexBufferPtr->Color = color;
		_Data.RectVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
		_Data.RectVertexBufferPtr->TexIndex = 0.0f;
		_Data.RectVertexBufferPtr->TilingFactor = 1.0f;
		_Data.RectVertexBufferPtr->EntityID = entityID;
		_Data.RectVertexBufferPtr++;

		_Data.RectVertexBufferPtr->Position = transform * _Data.RectVertexPositions[2];
		_Data.RectVertexBufferPtr->Color = color;
		_Data.RectVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
		_Data.RectVertexBufferPtr->TexIndex = 0.0f;
		_Data.RectVertexBufferPtr->TilingFactor = 1.0f;
		_Data.RectVertexBufferPtr->EntityID = entityID;
		_Data.RectVertexBufferPtr++;

		_Data.RectVertexBufferPtr->Position = transform * _Data.RectVertexPositions[3];
		_Data.RectVertexBufferPtr->Color = color;
		_Data.RectVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
		_Data.RectVertexBufferPtr->TexIndex = 0.0f;
		_Data.RectVertexBufferPtr->TilingFactor = 1.0f;
		_Data.RectVertexBufferPtr->EntityID = entityID;
		_Data.RectVertexBufferPtr++;

		_Data.RectIndexCount += 6;

		_Data.Stats.RectCount++;
	}

	void Renderer::R2D::DrawRect(const glm::mat4& transform, const RefPtr<Texture2D> texture, float tilingFactor, const glm::vec4& tintColor, uint32_t entityID)
	{
		IMAGINE_PROFILE_FUNCTION();

		constexpr size_t rectVertexCount = 4;
		constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

		if (_Data.RectIndexCount + 6 > Renderer2DData::MaxIndices) {
			NextBatch();
		}

		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < _Data.TextureSlotIndex; i++) {
			if (*_Data.TextureSlots[i].get() == *texture.get()) {
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f) {
			textureIndex = (float)_Data.TextureSlotIndex;
			_Data.TextureSlots[_Data.TextureSlotIndex] = texture;
			_Data.TextureSlotIndex++;
		}

		_Data.RectVertexBufferPtr->Position = transform * _Data.RectVertexPositions[0];
		_Data.RectVertexBufferPtr->Color = tintColor;
		_Data.RectVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
		_Data.RectVertexBufferPtr->TexIndex = textureIndex;
		_Data.RectVertexBufferPtr->TilingFactor = tilingFactor;
		_Data.RectVertexBufferPtr->EntityID = entityID;
		_Data.RectVertexBufferPtr++;

		_Data.RectVertexBufferPtr->Position = transform * _Data.RectVertexPositions[1];
		_Data.RectVertexBufferPtr->Color = tintColor;
		_Data.RectVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
		_Data.RectVertexBufferPtr->TexIndex = textureIndex;
		_Data.RectVertexBufferPtr->TilingFactor = tilingFactor;
		_Data.RectVertexBufferPtr->EntityID = entityID;
		_Data.RectVertexBufferPtr++;

		_Data.RectVertexBufferPtr->Position = transform * _Data.RectVertexPositions[2];
		_Data.RectVertexBufferPtr->Color = tintColor;
		_Data.RectVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
		_Data.RectVertexBufferPtr->TexIndex = textureIndex;
		_Data.RectVertexBufferPtr->TilingFactor = tilingFactor;
		_Data.RectVertexBufferPtr->EntityID = entityID;
		_Data.RectVertexBufferPtr++;

		_Data.RectVertexBufferPtr->Position = transform * _Data.RectVertexPositions[3];
		_Data.RectVertexBufferPtr->Color = tintColor;
		_Data.RectVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
		_Data.RectVertexBufferPtr->TexIndex = textureIndex;
		_Data.RectVertexBufferPtr->TilingFactor = tilingFactor;
		_Data.RectVertexBufferPtr->EntityID = entityID;
		_Data.RectVertexBufferPtr++;

		_Data.RectIndexCount += 6;

		_Data.Stats.RectCount++;
	}

	void Renderer::R2D::DrawRotatedRect(const glm::vec2& position, const glm::vec2& scale, float rotation, const glm::vec4& color)
	{
		DrawRotatedRect({position.x, position.y, 0.0f}, scale, rotation, color);
	}
	void Renderer::R2D::DrawRotatedRect(const glm::vec3& position, const glm::vec2& scale, float rotation, const glm::vec4& color)
	{
		IMAGINE_PROFILE_FUNCTION();

		if (_Data.RectIndexCount + 6 > Renderer2DData::MaxIndices) {
			NextBatch();
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { scale.x, scale.y, 1.0f });

		DrawRotatedRect(transform, color);
	}
	void Renderer::R2D::DrawRotatedRect(const glm::vec2& position, const glm::vec2& scale, float rotation, const RefPtr<Texture2D> texture, float tilingFactor, const glm::vec4& tintColor)
	{
		DrawRotatedRect({position.x, position.y, 0.0f}, scale, rotation, texture, tilingFactor, tintColor);
	}
	void Renderer::R2D::DrawRotatedRect(const glm::vec3& position, const glm::vec2& scale, float rotation, const RefPtr<Texture2D> texture, float tilingFactor, const glm::vec4& tintColor)
	{

		IMAGINE_PROFILE_FUNCTION();

		if (_Data.RectIndexCount + 6 > Renderer2DData::MaxIndices) {
			NextBatch();
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { scale.x, scale.y, 1.0f });

		DrawRotatedRect(transform, texture, tilingFactor, tintColor);
	}

	void Renderer::R2D::DrawRotatedRect(const glm::vec2& position, const glm::vec2& scale, float rotation, const RefPtr<SubTexture2D> subtexture, float tilingFactor, const glm::vec4& tintColor)
	{
		DrawRotatedRect({ position.x, position.y, 0.0f }, scale, rotation, subtexture, tilingFactor, tintColor);
	}
	void Renderer::R2D::DrawRotatedRect(const glm::vec3& position, const glm::vec2& scale, float rotation, const RefPtr<SubTexture2D> subtexture, float tilingFactor, const glm::vec4& tintColor)
	{

		IMAGINE_PROFILE_FUNCTION();

		if (_Data.RectIndexCount + 6 > Renderer2DData::MaxIndices) {
			NextBatch();
		}

		constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
		const glm::vec2* textureCoords = subtexture->GetTexCoords();
		const RefPtr<Texture2D> texture = subtexture->GetTexture();

		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < _Data.TextureSlotIndex; i++) {
			if (*_Data.TextureSlots[i].get() == *texture.get()) {
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f) {
			textureIndex = (float)_Data.TextureSlotIndex;
			_Data.TextureSlots[_Data.TextureSlotIndex] = texture;
			_Data.TextureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { scale.x, scale.y, 1.0f });

		_Data.RectVertexBufferPtr->Position = transform * _Data.RectVertexPositions[0];
		_Data.RectVertexBufferPtr->Color = color;
		_Data.RectVertexBufferPtr->TexCoord = textureCoords[0];
		_Data.RectVertexBufferPtr->TexIndex = textureIndex;
		_Data.RectVertexBufferPtr->TilingFactor = tilingFactor;
		_Data.RectVertexBufferPtr++;

		_Data.RectVertexBufferPtr->Position = transform * _Data.RectVertexPositions[1];
		_Data.RectVertexBufferPtr->Color = color;
		_Data.RectVertexBufferPtr->TexCoord = textureCoords[1];
		_Data.RectVertexBufferPtr->TexIndex = textureIndex;
		_Data.RectVertexBufferPtr->TilingFactor = tilingFactor;
		_Data.RectVertexBufferPtr++;

		_Data.RectVertexBufferPtr->Position = transform * _Data.RectVertexPositions[2];
		_Data.RectVertexBufferPtr->Color = color;
		_Data.RectVertexBufferPtr->TexCoord = textureCoords[2];
		_Data.RectVertexBufferPtr->TexIndex = textureIndex;
		_Data.RectVertexBufferPtr->TilingFactor = tilingFactor;
		_Data.RectVertexBufferPtr++;

		_Data.RectVertexBufferPtr->Position = transform * _Data.RectVertexPositions[3];
		_Data.RectVertexBufferPtr->Color = color;
		_Data.RectVertexBufferPtr->TexCoord = textureCoords[3];
		_Data.RectVertexBufferPtr->TexIndex = textureIndex;
		_Data.RectVertexBufferPtr->TilingFactor = tilingFactor;
		_Data.RectVertexBufferPtr++;

		_Data.RectIndexCount += 6;

		_Data.Stats.RectCount++;
	}

	void Renderer::R2D::DrawRotatedRect(const glm::mat4& transform, const glm::vec4& color)
	{
		_Data.RectVertexBufferPtr->Position = transform * _Data.RectVertexPositions[0];
		_Data.RectVertexBufferPtr->Color = color;
		_Data.RectVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
		_Data.RectVertexBufferPtr->TexIndex = 0.0f;
		_Data.RectVertexBufferPtr->TilingFactor = 1.0f;
		_Data.RectVertexBufferPtr++;

		_Data.RectVertexBufferPtr->Position = transform * _Data.RectVertexPositions[1];
		_Data.RectVertexBufferPtr->Color = color;
		_Data.RectVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
		_Data.RectVertexBufferPtr->TexIndex = 0.0f;
		_Data.RectVertexBufferPtr->TilingFactor = 1.0f;
		_Data.RectVertexBufferPtr++;

		_Data.RectVertexBufferPtr->Position = transform * _Data.RectVertexPositions[2];
		_Data.RectVertexBufferPtr->Color = color;
		_Data.RectVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
		_Data.RectVertexBufferPtr->TexIndex = 0.0f;
		_Data.RectVertexBufferPtr->TilingFactor = 1.0f;
		_Data.RectVertexBufferPtr++;

		_Data.RectVertexBufferPtr->Position = transform * _Data.RectVertexPositions[3];
		_Data.RectVertexBufferPtr->Color = color;
		_Data.RectVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
		_Data.RectVertexBufferPtr->TexIndex = 0.0f;
		_Data.RectVertexBufferPtr->TilingFactor = 1.0f;
		_Data.RectVertexBufferPtr++;

		_Data.RectIndexCount += 6;

		_Data.Stats.RectCount++;
	}

	void Renderer::R2D::DrawRotatedRect(const glm::mat4& transform, const RefPtr<Texture2D> texture, float tilingFactor, const glm::vec4& tintColor)
	{
		constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < _Data.TextureSlotIndex; i++) {
			if (*_Data.TextureSlots[i].get() == *texture.get()) {
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f) {
			textureIndex = (float)_Data.TextureSlotIndex;
			_Data.TextureSlots[_Data.TextureSlotIndex] = texture;
			_Data.TextureSlotIndex++;
		}

		_Data.RectVertexBufferPtr->Position = transform * _Data.RectVertexPositions[0];
		_Data.RectVertexBufferPtr->Color = color;
		_Data.RectVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
		_Data.RectVertexBufferPtr->TexIndex = textureIndex;
		_Data.RectVertexBufferPtr->TilingFactor = tilingFactor;
		_Data.RectVertexBufferPtr++;

		_Data.RectVertexBufferPtr->Position = transform * _Data.RectVertexPositions[1];
		_Data.RectVertexBufferPtr->Color = color;
		_Data.RectVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
		_Data.RectVertexBufferPtr->TexIndex = textureIndex;
		_Data.RectVertexBufferPtr->TilingFactor = tilingFactor;
		_Data.RectVertexBufferPtr++;

		_Data.RectVertexBufferPtr->Position = transform * _Data.RectVertexPositions[2];
		_Data.RectVertexBufferPtr->Color = color;
		_Data.RectVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
		_Data.RectVertexBufferPtr->TexIndex = textureIndex;
		_Data.RectVertexBufferPtr->TilingFactor = tilingFactor;
		_Data.RectVertexBufferPtr++;

		_Data.RectVertexBufferPtr->Position = transform * _Data.RectVertexPositions[3];
		_Data.RectVertexBufferPtr->Color = color;
		_Data.RectVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
		_Data.RectVertexBufferPtr->TexIndex = textureIndex;
		_Data.RectVertexBufferPtr->TilingFactor = tilingFactor;
		_Data.RectVertexBufferPtr++;

		_Data.RectIndexCount += 6;

		_Data.Stats.RectCount++;
	}

	void Renderer::R2D::DrawCircle(const glm::mat4& transform, const glm::vec4& color, float thickness, float fade, uint32_t entityID)
	{
		IMAGINE_PROFILE_FUNCTION();

		//TODO: IMPLMENENT FOR CIRCLES
		//if (_Data.RectIndexCount + 6 > Renderer2DData::MaxIndices) {
		//	NextBatch();
		//}
		for (size_t i = 0; i < 4; ++i) {
			_Data.CircleVertexBufferPtr->Position = transform * _Data.RectVertexPositions[i];
			_Data.CircleVertexBufferPtr->LocalPosition = _Data.RectVertexPositions[i] * 2.0f;
			_Data.CircleVertexBufferPtr->Color = color;
			_Data.CircleVertexBufferPtr->Thickness = thickness;
			_Data.CircleVertexBufferPtr->Fade = fade;
			_Data.CircleVertexBufferPtr->EntityID = entityID;
			_Data.CircleVertexBufferPtr++;
		}

		_Data.CircleIndexCount += 6;

		//TODO: seperate rect count and circle count
		_Data.Stats.RectCount++;
	}

	void Renderer::R2D::DrawEditorLine(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4& color, float thickness)
	{
		_Data.EditorLineVertexBufferPtr->Position = p0;
		_Data.EditorLineVertexBufferPtr->Color = color;
		_Data.EditorLineVertexBufferPtr++;

		_Data.EditorLineVertexBufferPtr->Position = p1;
		_Data.EditorLineVertexBufferPtr->Color = color;
		_Data.EditorLineVertexBufferPtr++;

		_Data.EditorLineVertexCount += 2;
	}

	void Renderer::R2D::DrawEditorRect(const glm::vec3& position, const glm::vec2& scale, const glm::vec4& color)
	{
		glm::vec3 p0 = glm::vec3(position.x - scale.x * 0.5f, position.y - scale.y * 0.5f, position.z);
		glm::vec3 p1 = glm::vec3(position.x + scale.x * 0.5f, position.y - scale.y * 0.5f, position.z);
		glm::vec3 p2 = glm::vec3(position.x + scale.x * 0.5f, position.y + scale.y * 0.5f, position.z);
		glm::vec3 p3 = glm::vec3(position.x - scale.x * 0.5f, position.y + scale.y * 0.5f, position.z);

		DrawEditorLine(p0, p1, color);
		DrawEditorLine(p1, p2, color);
		DrawEditorLine(p2, p3, color);
		DrawEditorLine(p3, p0, color);
	}

	void Renderer::R2D::DrawEditorRect(const glm::mat4& transform, const glm::vec4& color)
	{
		glm::vec3 lineVertices[4];
		for (size_t i = 0; i < 4; ++i) {
			lineVertices[i] = transform * _Data.RectVertexPositions[i];
		}

		DrawEditorLine(lineVertices[0], lineVertices[1], color);
		DrawEditorLine(lineVertices[1], lineVertices[2], color);
		DrawEditorLine(lineVertices[2], lineVertices[3], color);
		DrawEditorLine(lineVertices[3], lineVertices[0], color);
	}

	void Renderer::R2D::DrawSprite(const glm::mat4& transform, const C_SpriteRenderer& src, uint32_t entityID)
	{
		if (src.Texture) {
			DrawRect(transform, src.Texture, src.TilingFactor, src.Color, entityID);
		}
		else {
			DrawRect(transform, src.Color, entityID);
		}
	}

	const float Renderer::R2D::GetEditorLineThickness()
	{
		return _Data.EditorLineThickness;
	}

	void Renderer::R2D::SetEditorLineThickness(float thickness)
	{
		_Data.EditorLineThickness = thickness;
	}

	Renderer::R2D::Statistics Renderer::R2D::GetStats() {
		return _Data.Stats;
	}
	void Renderer::R2D::ResetStats() {
		memset(&_Data.Stats, 0, sizeof(Statistics));
	}
	//==========2D==========2D==========2D==========2D==========2D==========2D==========2D==========2D==========2D==========2D
}