#pragma once

#include "Imagine.h"

class MyApp2D : public IM::Layer
{
public:
	MyApp2D();
	~MyApp2D() = default;
	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate(float dt) override;
	void OnImguiRender() override;
private:
	IM::OrthographicCameraController _CameraController;

	//TEMP
	IM::RefPtr<IM::Shader> _Shader;
	IM::RefPtr<IM::VertexArray> _VertexArray;
	glm::vec4 _SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };

	IM::RefPtr<IM::Texture2D> _Texture;

	std::vector<IM::ProfileResult> _ProfileResults;
};

