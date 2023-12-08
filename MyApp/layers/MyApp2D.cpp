#include "MyApp2D.h"

#include "Platform/OpenGL/OpenGLShader.h"

#include "Imgui/imgui.h"

MyApp2D::MyApp2D()
	: Layer("MyApp2D"), _CameraController(1280.0f / 720.0f) {
}

void MyApp2D::OnAttach()
{
	_VertexArray = IM::VertexArray::Create();

	float vertices2[4 * 3] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.5f, 0.5f, 0.0f, 
		-0.5f, 0.5f, 0.0f
	};

	IM::RefPtr<IM::VertexBuffer> squareVB;
	squareVB.reset(IM::VertexBuffer::Create(vertices2, sizeof(vertices2)));
	squareVB->SetLayout({
		{IM::ShaderDataType::Float3, "a_Position"}
		});
	_VertexArray->AddVertexBuffer(squareVB);

	uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
	IM::RefPtr<IM::IndexBuffer> squareIB;
	squareIB = IM::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
	_VertexArray->SetIndexBuffer(squareIB);

	_Shader = IM::Shader::Create("assets/shaders/flatColor.glsl");
}

void MyApp2D::OnDetach()
{
}

void MyApp2D::OnUpdate(float dt)
{
	_CameraController.OnUpdate(dt);

	IM::RenderCommand::SetClearColor({ 0.4f, 0.3f, 0.3f, 1.0f });
	IM::RenderCommand::Clear();

	//Beginning of the scene where we tell renderer what to do for the scene

	IM::Renderer::BeginScene(_CameraController.GetCamera());

	std::dynamic_pointer_cast<IM::OpenGLShader>(_Shader)->Bind();
	std::dynamic_pointer_cast<IM::OpenGLShader>(_Shader)->SetUniform("u_Color", _SquareColor);

	C_Transform _SquareTransform;
	_SquareTransform.Transform = glm::scale(glm::mat4(1.0f), glm::vec3(1.5f));
	IM::Renderer::Submit(_Shader, _VertexArray, _SquareTransform);

	IM::Renderer::EndScene();
}

void MyApp2D::OnImguiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit3("Square Color", glm::value_ptr(_SquareColor));
	ImGui::End();
}
