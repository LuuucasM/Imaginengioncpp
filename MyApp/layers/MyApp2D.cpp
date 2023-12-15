#include "MyApp2D.h"
#include "Imgui/imgui.h"

MyApp2D::MyApp2D()
	: Layer("MyApp2D"), _CameraController(1280.0f / 720.0f) {
}

void MyApp2D::OnAttach()
{
	_Texture = IM::Texture2D::Create("assets/textures/Checkerboard.png");
}

void MyApp2D::OnDetach()
{

}

void MyApp2D::OnUpdate(float dt)
{
	IM::ProfileTimer timer("MyApp2D::OnUpdate", [&](IM::ProfileResult profileResult) {_ProfileResults.push_back(profileResult); });

	_CameraController.OnUpdate(dt);

	IM::Renderer::SetClearColor({ 0.4f, 0.3f, 0.3f, 1.0f });
	IM::Renderer::Clear();

	//Beginning of the scene where we tell renderer what to do for the scene

	IM::Renderer::R2D::BeginScene(_CameraController.GetCamera());

	IM::Renderer::R2D::DrawRect({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
	IM::Renderer::R2D::DrawRect({ 0.5f, -0.5f }, { 1.0f, 1.0f }, { 0.3f, 0.8f, 0.2f, 1.0f });

	IM::Renderer::R2D::DrawRect({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, _Texture);

	IM::Renderer::R2D::EndScene();
}

void MyApp2D::OnImguiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit3("Square Color", glm::value_ptr(_SquareColor));

	for (auto& result : _ProfileResults) {
		char label[50];
		strcpy(label, result.Name);
		strcat(label, "  %.3fms");
		ImGui::Text(label, result.Time);
	}
	_ProfileResults.clear();

	ImGui::End();
}
