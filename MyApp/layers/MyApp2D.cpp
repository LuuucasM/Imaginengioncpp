#include "MyApp2D.h"

#include "Platform/OpenGL/OpenGLShader.h"

#include "Imgui/imgui.h"

MyApp2D::MyApp2D()
	: Layer("MyApp2D"), _CameraController(1280.0f / 720.0f) {
}

void MyApp2D::OnAttach()
{

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

	IM::Renderer::R2D::BeginScene(_CameraController.GetCamera());

	IM::Renderer::R2D::DrawRect({ 0.0f, 0.0f }, { 1.0f, 1.0f }, { 0.8f, 0.2f, 0.3f, 1.0f });

	IM::Renderer::R2D::EndScene();

	//TODO _shader->setInput
	//std::dynamic_pointer_cast<IM::OpenGLShader>(_Shader)->Bind();
	//std::dynamic_pointer_cast<IM::OpenGLShader>(_Shader)->SetUniform("u_Color", _SquareColor);
}

void MyApp2D::OnImguiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit3("Square Color", glm::value_ptr(_SquareColor));
	ImGui::End();
}
