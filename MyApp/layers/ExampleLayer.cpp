#include "ExampleLayer.h"

#include "Imgui/imgui.h"

ExampleLayer::ExampleLayer()
	: Layer("Example"), _CameraController(1280.0f / 720.0f) {
	_VertexArray = IM::VertexArray::Create();


	float vertices[3 * 7] = {
		-0.5f, -0.5f, 0.0f, 0.8f, 0.1f, 0.2f, 1.0f,
		0.5f, -0.5f, 0.0f, 0.4f, 0.8f, 0.1f, 1.0f,
		0.0f, 0.5f, 0.0f, 0.1f, 0.3f, 0.8f, 1.0f
	};

	IM::RefPtr<IM::VertexBuffer> vertexBuffer;
	vertexBuffer.reset(IM::VertexBuffer::Create(vertices, sizeof(vertices)));
	IM::BufferLayout layout = {
		{IM::ShaderDataType::Float3, "a_Position"},
		{IM::ShaderDataType::Float4, "a_Color"}
	};
	vertexBuffer->SetLayout(layout);
	_VertexArray->AddVertexBuffer(vertexBuffer);

	//Index Buffer
	uint32_t indices[3] = { 0, 1, 2 };
	IM::RefPtr<IM::IndexBuffer> indexBuffer;
	indexBuffer = IM::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
	_VertexArray->SetIndexBuffer(indexBuffer);

	_SquareVA = IM::VertexArray::Create();

	float vertices2[4 * 5] = {
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.0f, 0.0, 1.0f
	};

	IM::RefPtr<IM::VertexBuffer> squareVB;
	squareVB.reset(IM::VertexBuffer::Create(vertices2, sizeof(vertices2)));
	squareVB->SetLayout({
		{IM::ShaderDataType::Float3, "a_Position"},
		{IM::ShaderDataType::Float2, "a_TexCoord"}
		});
	_SquareVA->AddVertexBuffer(squareVB);

	uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
	IM::RefPtr<IM::IndexBuffer> squareIB;
	squareIB = IM::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
	_SquareVA->SetIndexBuffer(squareIB);
	//TEMPORARY------------------
	std::string vertexSrc = R"(
		#version 330 core
			
		layout(location = 0) in vec3 a_Position;
		layout(location = 1) in vec4 a_Color;

		uniform mat4 u_ViewProjection;
		uniform mat4 u_Transform;

		out vec3 v_Position;
		out vec4 v_Color;

		void main() {
			v_Position = a_Position;
			v_Color = a_Color;
			gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
		}
	)";
	std::string fragmentSrc = R"(
		#version 330 core
			
		layout(location = 0) out vec4 color;

		in vec3 v_Position;
		in vec4 v_Color;

		void main() {
			color = vec4(0.2, 0.3, 0.8, 1.0);
			color = v_Color;
		}
	)";

	std::string vertexSrc2 = R"(
		#version 330 core
			
		layout(location = 0) in vec3 a_Position;

		uniform mat4 u_ViewProjection;
		uniform mat4 u_Transform;

			
		out vec3 v_Position;			

		void main() {
			v_Position = a_Position;
			gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
		}
	)";
	std::string fragmentSrc2 = R"(
		#version 330 core
			
		layout(location = 0) out vec4 color;

		in vec3 v_Position;

		uniform vec4 u_Color;
			
		void main() {
			color = u_Color;
		}
	)";
	_Shader = IM::Shader::Create("TriangleShader", vertexSrc, fragmentSrc);
	_Shader2 = IM::Shader::Create("SquareShader", vertexSrc2, fragmentSrc2);

	//

	_TextureShader = IM::Shader::Create("assets/shaders/Texture.glsl");

	//IM::Shader::Create(textureShaderVertexSrc, textureShaderFragSrc);

	_Texture = IM::Texture2D::Create("assets/textures/Checkerboard.png");
	_ZealotTexture = IM::Texture2D::Create("assets/textures/zealot.png");

	_TextureShader->Bind();
	_TextureShader->SetValue("u_Texture", 0);
}

void ExampleLayer::OnUpdate(float dt){

	//Update
	_CameraController.OnUpdate(dt);

	//Render
	if (IM::Input::IsKeyPressed(IMAGINE_KEY_J))
		_SquareTransform.Transform[3].x -= _CameraMoveSpeed * dt;
	if (IM::Input::IsKeyPressed(IMAGINE_KEY_L))
		_SquareTransform.Transform[3].x += _CameraMoveSpeed * dt;
	if (IM::Input::IsKeyPressed(IMAGINE_KEY_I))
		_SquareTransform.Transform[3].y += _CameraMoveSpeed * dt;
	if (IM::Input::IsKeyPressed(IMAGINE_KEY_K))
		_SquareTransform.Transform[3].y -= _CameraMoveSpeed * dt;

	IM::Renderer::SetClearColor({ 0.4f, 0.3f, 0.3f, 1.0f });
	IM::Renderer::Clear();

	//Beginning of the scene where we tell renderer what to do for the scene

	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

	IM::Renderer::R3D::BeginScene(_CameraController.GetCamera());

	_Shader2->Bind();
	_Shader2->SetValue("u_Color", _SquareColor);

	for (int y = 0; y < 20; ++y) {
		for (int x = 0; x < 20; ++x) {
			glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
			C_Transform trans;
			trans.Transform = glm::translate(glm::mat4(1.0f), pos) * scale;
			IM::Renderer::R3D::Submit(_Shader2, _SquareVA, trans);
		}
	}
	_SquareTransform2.Transform = glm::scale(glm::mat4(1.0f), glm::vec3(1.5f));
	_Texture->Bind();
	IM::Renderer::R3D::Submit(_TextureShader, _SquareVA, _SquareTransform2);

	_ZealotTransform.Transform = glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)), glm::vec3(0.25f, -0.25f, 0.25f));
	_ZealotTexture->Bind();
	IM::Renderer::R3D::Submit(_TextureShader, _SquareVA, _SquareTransform2);

	//THIS IS THE TRIANGLE
	//IM::Renderer::Submit(_Shader, _VertexArray, _SquareTransform2);

	//EndScene tells the renderer we are done submitting and it can do its thing now
	IM::Renderer::R3D::EndScene();
	//flush the render queue
	//Renderer::Flush();
}

void ExampleLayer::OnImguiRender(){
	ImGui::Begin("Settings");
	ImGui::ColorEdit3("Square Color", glm::value_ptr(_SquareColor));
	ImGui::End();
}