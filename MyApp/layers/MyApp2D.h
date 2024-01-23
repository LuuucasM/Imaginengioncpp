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
	IM::RefPtr<IM::FrameBuffer> _FrameBuffer;

	IM::RefPtr<IM::Texture2D> _Texture;
	IM::RefPtr<IM::Texture2D> _TextureZealot;
	IM::RefPtr<IM::Texture2D> _TextureSpriteSheet;
	IM::RefPtr<IM::SubTexture2D> _TextureSample;

	uint32_t _MapWidth, _MapHeight;
};