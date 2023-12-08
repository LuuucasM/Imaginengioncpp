#pragma once

#include "Imagine.h"

class ExampleLayer : public IM::Layer {
public:
	ExampleLayer();
	~ExampleLayer() = default;
	void OnUpdate(float dt) override;
	void OnImguiRender() override;
private:
	//FOR MAKING A TRIANGLE --------------
	IM::RefPtr<IM::VertexArray> _VertexArray;
	//FOR MAKING A SQUARE
	IM::RefPtr<IM::VertexArray> _SquareVA;
	//TEMPORARY---------------
	IM::RefPtr<IM::Shader> _Shader;
	IM::RefPtr<IM::Shader> _Shader2;
	IM::RefPtr<IM::Shader> _TextureShader;

	IM::RefPtr<IM::Texture> _Texture;
	IM::RefPtr<IM::Texture> _ZealotTexture;

	IM::OrthographicCameraController _CameraController;


	glm::vec3 _CameraPos = glm::vec3(0.0f);
	float _CameraMoveSpeed = 5.0f;
	float _CameraRot = 0.0f;
	float _CameraRotSpeed = 30.0f;

	C_Transform _SquareTransform;
	C_Transform _SquareTransform2;
	C_Transform _ZealotTransform;

	glm::vec4 _SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
};