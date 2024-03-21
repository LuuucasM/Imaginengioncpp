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

	struct Renderer2DData {

		static const uint32_t MaxRect = 10000; //TODO: MAKE CLASS RENDERER CAPABILITIES
		static const uint32_t MaxVerticies = MaxRect * 4;
		static const uint32_t MaxIndices = MaxRect * 6;
		static const uint32_t MaxTextureSlots = 32; //TODO: MAKE CLASS RENDERER CAPABILITIES

		RefPtr<VertexArray> _VertexArray;
		RefPtr<VertexBuffer> _VertexBuffer;
		RefPtr<Shader> _TextureShader;
		RefPtr<Texture2D> _WhiteTexture;

		uint32_t RectIndexCount = 0;
		RectVertex* RectVertexBufferBase = nullptr;
		RectVertex* RectVertexBufferPtr = nullptr;

		std::array<RefPtr<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1; // texture slot 0 == white texture

		glm::vec4 RectVertexPositions[4];

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

		_Data._VertexArray = VertexArray::Create();

		//_Data._Texture Shader = Shader::Create(filepath)
		//_Data.VertexBuffer->VertexBuffer::Create(blah)
		//_Data._VertexBuffer->SetLayout(Shader.GetLayout);

		int samplers[_Data.MaxTextureSlots];
		for (uint32_t i = 0; i < _Data.MaxTextureSlots; i++) {
			samplers[i] = i;
		}

		_Data._TextureShader = Shader::Create("assets/shaders/Texture.glsl");

		_Data._VertexBuffer = VertexBuffer::Create(_Data.MaxVerticies * sizeof(RectVertex));
		_Data._VertexBuffer->SetLayout(_Data._TextureShader->GetLayout());
		_Data._VertexBuffer->SetStride(_Data._TextureShader->GetStride());

		_Data._VertexArray->AddVertexBuffer(_Data._VertexBuffer);

		_Data.RectVertexBufferBase = new RectVertex[_Data.MaxVerticies];

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
		_Data._VertexArray->SetIndexBuffer(rectIB);
		delete[] rectIndices;

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

		_Data._TextureShader->Bind();
		_Data._TextureShader->SetValue("u_ViewProjection", camera.GetViewProjectionMatrix());

		_Data.RectIndexCount = 0;

		_Data.RectVertexBufferPtr = _Data.RectVertexBufferBase;

		_Data.TextureSlotIndex = 1;

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
		//nothing right now :)
		IMAGINE_PROFILE_FUNCTION();

		__int64 dataSize = (uint8_t*)_Data.RectVertexBufferPtr - (uint8_t*)_Data.RectVertexBufferBase;
		_Data._VertexBuffer->SetData(_Data.RectVertexBufferBase, dataSize);

		FlushScene();
	}

	void Renderer::R2D::StartBatch() {
		_Data.RectIndexCount = 0;

		_Data.RectVertexBufferPtr = _Data.RectVertexBufferBase;

		_Data.TextureSlotIndex = 1;
	}
	void Renderer::R2D::FlushScene()
	{
		//Bind Textures;
		for (uint32_t i = 0; i < _Data.TextureSlotIndex; i++){
			_Data.TextureSlots[i]->Bind(i);
		}

		_Data._TextureShader->Bind();
		RenderCommand::DrawIndexed(_Data._VertexArray, _Data.RectIndexCount);
		++_Data.Stats.DrawCalls;
	}

	void Renderer::R2D::FlushAndReset() {
		EndScene();
		StartBatch();
	}
	void Renderer::R2D::DrawRect(const glm::vec2& position, const glm::vec2& scale, const glm::vec4& color)
	{
		DrawRect({ position.x, position.y, 0.0f }, scale, color);
	}
	void Renderer::R2D::DrawRect(const glm::vec3& position, const glm::vec2& scale, const glm::vec4& color)
	{

		IMAGINE_PROFILE_FUNCTION();

		if (_Data.RectIndexCount + 6 > Renderer2DData::MaxIndices) {
			FlushAndReset();
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
			FlushAndReset();
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
			FlushAndReset();
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
			FlushAndReset();
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
			FlushAndReset();
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
		_Data.RectVertexBufferPtr->Color = color;
		_Data.RectVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
		_Data.RectVertexBufferPtr->TexIndex = textureIndex;
		_Data.RectVertexBufferPtr->TilingFactor = tilingFactor;
		_Data.RectVertexBufferPtr->EntityID = entityID;
		_Data.RectVertexBufferPtr++;

		_Data.RectVertexBufferPtr->Position = transform * _Data.RectVertexPositions[1];
		_Data.RectVertexBufferPtr->Color = color;
		_Data.RectVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
		_Data.RectVertexBufferPtr->TexIndex = textureIndex;
		_Data.RectVertexBufferPtr->TilingFactor = tilingFactor;
		_Data.RectVertexBufferPtr->EntityID = entityID;
		_Data.RectVertexBufferPtr++;

		_Data.RectVertexBufferPtr->Position = transform * _Data.RectVertexPositions[2];
		_Data.RectVertexBufferPtr->Color = color;
		_Data.RectVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
		_Data.RectVertexBufferPtr->TexIndex = textureIndex;
		_Data.RectVertexBufferPtr->TilingFactor = tilingFactor;
		_Data.RectVertexBufferPtr->EntityID = entityID;
		_Data.RectVertexBufferPtr++;

		_Data.RectVertexBufferPtr->Position = transform * _Data.RectVertexPositions[3];
		_Data.RectVertexBufferPtr->Color = color;
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
			FlushAndReset();
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
			FlushAndReset();
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
			FlushAndReset();
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

	void Renderer::R2D::DrawSprite(const glm::mat4& transform, const C_SpriteRenderer& src, uint32_t entityID)
	{
		DrawRect(transform, src.Color, entityID);
	}

	Renderer::R2D::Statistics Renderer::R2D::GetStats() {
		return _Data.Stats;
	}
	void Renderer::R2D::ResetStats() {
		memset(&_Data.Stats, 0, sizeof(Statistics));
	}
	//==========2D==========2D==========2D==========2D==========2D==========2D==========2D==========2D==========2D==========2D
}