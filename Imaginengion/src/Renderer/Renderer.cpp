#include "impch.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace IM {
	Renderer::R3D::SceneData* Renderer::R3D::_SceneData = new Renderer::R3D::SceneData();

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
		std::dynamic_pointer_cast<OpenGLShader>(shader)->SetUniform("u_ViewProjection", _SceneData->ViewProjectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->SetUniform("u_Transform", transform.Transform);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
	//==========3D==========3D==========3D==========3D==========3D==========3D==========3D==========3D==========3D==========3D


	//==========2D==========2D==========2D==========2D==========2D==========2D==========2D==========2D==========2D==========2D
	struct Renderer2DData {
		RefPtr<VertexArray> _VertexArray;
		RefPtr<Shader> _Shader;
	};

	static Renderer2DData* _Data;

	void Renderer::R2D::Init() {
		_Data = new Renderer2DData();
		_Data->_VertexArray = VertexArray::Create();

		float vertices2[4 * 3] = {
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.5f, 0.5f, 0.0f,
			-0.5f, 0.5f, 0.0f
		};

		RefPtr<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::Create(vertices2, sizeof(vertices2)));
		squareVB->SetLayout({
			{ShaderDataType::Float3, "a_Position"}
			});
		_Data->_VertexArray->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		RefPtr<IndexBuffer> squareIB;
		squareIB = IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
		_Data->_VertexArray->SetIndexBuffer(squareIB);

		_Data->_Shader = Shader::Create("assets/shaders/flatColor.glsl");
	}

	void Renderer::R2D::Shutdown() {
		delete _Data;
	}

	void Renderer::R2D::BeginScene(const OrthographicCamera& camera)
	{
		std::dynamic_pointer_cast<OpenGLShader>(_Data->_Shader)->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(_Data->_Shader)->SetUniform("u_ViewProjection", camera.GetViewProjectionMatrix());
		std::dynamic_pointer_cast<OpenGLShader>(_Data->_Shader)->SetUniform("u_Transform", glm::mat4(1.0f));
	}
	void Renderer::R2D::EndScene()
	{
		//nothing right now :)
	}
	void Renderer::R2D::DrawRect(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawRect({ position.x, position.y, 0.0f }, size, color);
	}
	void Renderer::R2D::DrawRect(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		std::dynamic_pointer_cast<OpenGLShader>(_Data->_Shader)->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(_Data->_Shader)->SetUniform("u_Color", color);

		_Data->_VertexArray->Bind();
		RenderCommand::DrawIndexed(_Data->_VertexArray);
	}
	//==========2D==========2D==========2D==========2D==========2D==========2D==========2D==========2D==========2D==========2D
}