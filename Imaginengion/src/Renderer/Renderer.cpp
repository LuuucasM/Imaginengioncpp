#include "impch.h"

#include "Renderer.h"

#include <glm/gtc/matrix_transform.hpp>

namespace IM {
	ScopePtr<Renderer::R3D::SceneData> Renderer::R3D::_SceneData = CreateScopePtr<Renderer::R3D::SceneData>();

	void Renderer::Init() {
		RenderCommand::Init();
		Renderer::R3D::Init();
		Renderer::R2D::Init();
	}

	void Renderer::Shutdown() {
		Renderer::R3D::Shutdown();
		Renderer::R2D::Shutdown();
	}

	void Renderer::OnWindowResize(int width, int height){
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
		shader->SetValue("u_Transform", transform.Transform);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
	//==========3D==========3D==========3D==========3D==========3D==========3D==========3D==========3D==========3D==========3D


	//==========2D==========2D==========2D==========2D==========2D==========2D==========2D==========2D==========2D==========2D
	struct Renderer2DData {
		RefPtr<VertexArray> _VertexArray;
		RefPtr<Shader> _Shader;
		RefPtr<Shader> _TextureShader;
	};

	static Renderer2DData* _Data;

	void Renderer::R2D::Init() {
		_Data = new Renderer2DData();
		_Data->_VertexArray = VertexArray::Create();

		float vertices2[4 * 5] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f, 0.5f, 0.0f, 0.0f, 1.0f
		};

		RefPtr<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::Create(vertices2, sizeof(vertices2)));
		squareVB->SetLayout({
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float2, "a_TexCoord"}
			});
		_Data->_VertexArray->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		RefPtr<IndexBuffer> squareIB;
		squareIB = IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
		_Data->_VertexArray->SetIndexBuffer(squareIB);

		_Data->_Shader = Shader::Create("assets/shaders/flatColor.glsl");
		_Data->_TextureShader = Shader::Create("assets/shaders/Texture.glsl");
		_Data->_TextureShader->Bind();
		_Data->_TextureShader->SetValue("u_Texture", 0); 
	}

	void Renderer::R2D::Shutdown() {
		delete _Data;
	}

	void Renderer::R2D::BeginScene(const OrthographicCamera& camera)
	{
		_Data->_Shader->Bind();
		_Data->_Shader->SetValue("u_ViewProjection", camera.GetViewProjectionMatrix());

		_Data->_TextureShader->Bind();
		_Data->_TextureShader->SetValue("u_ViewProjection", camera.GetViewProjectionMatrix());

	}
	void Renderer::R2D::EndScene()
	{
		//nothing right now :)
	}
	void Renderer::R2D::DrawRect(const glm::vec2& position, const glm::vec2& scale, const glm::vec4& color)
	{
		DrawRect({ position.x, position.y, 0.0f }, scale, color);
	}
	void Renderer::R2D::DrawRect(const glm::vec3& position, const glm::vec2& scale, const glm::vec4& color)
	{
		_Data->_Shader->Bind();
		_Data->_Shader->SetValue("u_Color", color);

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), {scale.x, scale.y, 1.0f});
		_Data->_Shader->SetValue("u_Transform", transform);

		_Data->_VertexArray->Bind();
		RenderCommand::DrawIndexed(_Data->_VertexArray);
	}
	void Renderer::R2D::DrawRect(const glm::vec2& position, const glm::vec2& scale, const RefPtr<Texture2D> texture)
	{
		DrawRect({ position.x, position.y, 0.0f }, scale, texture);
	}
	void Renderer::R2D::DrawRect(const glm::vec3& position, const glm::vec2& scale, const RefPtr<Texture2D> texture)
	{
		_Data->_TextureShader->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { scale.x, scale.y, 1.0f });
		_Data->_TextureShader->SetValue("u_Transform", transform);

		texture->Bind();

		_Data->_VertexArray->Bind();
		RenderCommand::DrawIndexed(_Data->_VertexArray);
	}
	//==========2D==========2D==========2D==========2D==========2D==========2D==========2D==========2D==========2D==========2D
}