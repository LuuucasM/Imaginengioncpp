#include "Imagine.h"

class ExampleLayer : public IM::Layer {
public:
	ExampleLayer() 
		: Layer("Example"), _Camera(-1.6f, 1.6f, -0.9f, 0.9f) {

		_VertexArray.reset(IM::VertexArray::Create());


		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.1f, 0.2f, 1.0f,
			0.5f, -0.5f, 0.0f, 0.4f, 0.8f, 0.1f, 1.0f,
			0.0f, 0.5f, 0.0f, 0.1f, 0.3f, 0.8f, 1.0f
		};

		std::shared_ptr<IM::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(IM::VertexBuffer::Create(vertices, sizeof(vertices)));
		IM::BufferLayout layout = {
			{IM::ShaderDataType::Float3, "a_Position"},
			{IM::ShaderDataType::Float4, "a_Color"}
		};
		vertexBuffer->SetLayout(layout);
		_VertexArray->AddVertexBuffer(vertexBuffer);

		//Index Buffer
		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<IM::IndexBuffer> indexBuffer;
		indexBuffer.reset(IM::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		_VertexArray->SetIndexBuffer(indexBuffer);

		_SquareVA.reset(IM::VertexArray::Create());

		float vertices2[4 * 3] = {
			-0.75f, -0.75f, 0.0f,
			0.75f, -0.75f, 0.0f,
			0.75f, 0.75f, 0.0f,
			-0.75f, 0.75f, 0.0f,
		};

		std::shared_ptr<IM::VertexBuffer> squareVB;
		squareVB.reset(IM::VertexBuffer::Create(vertices2, sizeof(vertices2)));
		squareVB->SetLayout({
			{IM::ShaderDataType::Float3, "a_Position"},
			});
		_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<IM::IndexBuffer> squareIB;
		squareIB.reset(IM::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		_SquareVA->SetIndexBuffer(squareIB);
		//TEMPORARY------------------
		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;

			out vec3 v_Position;
			out vec4 v_Color;

			void main() {
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
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
			
			out vec3 v_Position;			

			void main() {
				v_Position = a_Position;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
			}
		)";
		std::string fragmentSrc2 = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			
			void main() {
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
			}
		)";
		_Shader.reset(new IM::Shader(vertexSrc, fragmentSrc));
		_Shader2.reset(new IM::Shader(vertexSrc2, fragmentSrc2));
	}

	void OnUpdate(float dt) override {

		if (IM::Input::IsKeyPressed(IMAGINE_KEY_LEFT))
			_CameraPos.x -= _CameraMoveSpeed * dt;
		if (IM::Input::IsKeyPressed(IMAGINE_KEY_RIGHT))
			_CameraPos.x += _CameraMoveSpeed * dt;
		if (IM::Input::IsKeyPressed(IMAGINE_KEY_UP))
			_CameraPos.y += _CameraMoveSpeed * dt;
		if (IM::Input::IsKeyPressed(IMAGINE_KEY_DOWN))
			_CameraPos.y -= _CameraMoveSpeed * dt;

		if (IM::Input::IsKeyPressed(IMAGINE_KEY_A))
			_CameraRot += _CameraMoveSpeed * dt;
		if (IM::Input::IsKeyPressed(IMAGINE_KEY_D))
			_CameraRot -= _CameraMoveSpeed * dt;

		IM::RenderCommand::SetClearColor({ 0.4f, 0.3f, 0.3f, 1.0f });
		IM::RenderCommand::Clear();

		_Camera.SetPosition(_CameraPos);
		_Camera.SetRotation(_CameraRot);

		//Beginning of the scene where we tell renderer what to do for the scene
		IM::Renderer::BeginScene(_Camera);

		IM::Renderer::Submit(_Shader2, _SquareVA);
		IM::Renderer::Submit(_Shader, _VertexArray);

		//EndScene tells the renderer we are done submitting and it can do its thing now
		IM::Renderer::EndScene();
		//flush the render queue
		//Renderer::Flush();
	}
public:
	void OnKeyPresedEvent() {

	}
private:
	//FOR MAKING A TRIANGLE --------------
	std::shared_ptr<IM::VertexArray> _VertexArray;
	//FOR MAKING A SQUARE
	std::shared_ptr<IM::VertexArray> _SquareVA;
	//TEMPORARY---------------
	std::shared_ptr<IM::Shader> _Shader;
	std::shared_ptr<IM::Shader> _Shader2;

	IM::OrthographicCamera _Camera;


	glm::vec3 _CameraPos = glm::vec3(0.0f);
	float _CameraMoveSpeed = 5.0f;
	float _CameraRot = 0.0f;
	float _CameraRotSpeed = 30.0f;

};

class MyApp : public IM::Application {
public:
	MyApp() {
		PushLayer(new ExampleLayer());
	}
	~MyApp() {

	}
};

IM::Application* IM::CreateApplication() {
	return new MyApp();
}