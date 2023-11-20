#include "impch.h"
#include "Application.h"


#include "Renderer/BufferLayout.h"

#include "Renderer/Renderer.h"

#include "glm/glm.hpp"

namespace IM {

	Application* Application::_Instance = nullptr;

	Application::Application() 
		: _Camera(-1.6f, 1.6f, -0.9f, 0.9f){
		//Application is singleton so check it does not already exist
		IMAGINE_CORE_ASSERT(!_Instance, "Application already exists!");
		_Instance = this;

		//create window and bind windowcloseevent to application function
		_Window = std::unique_ptr<Window>(Window::Create());
		_Window->WindowCloseEvent.AddListener<Application>(this, &Application::OnWindowCloseEvent);

		_ImguiLayer = new ImguiLayer();
		PushOverlay(_ImguiLayer);

		_VertexArray.reset(VertexArray::Create());


		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.1f, 0.2f, 1.0f,
			0.5f, -0.5f, 0.0f, 0.4f, 0.8f, 0.1f, 1.0f,
			0.0f, 0.5f, 0.0f, 0.1f, 0.3f, 0.8f, 1.0f
		};

		std::shared_ptr<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
		BufferLayout layout = {
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float4, "a_Color"}
		};
		vertexBuffer->SetLayout(layout);
		_VertexArray->AddVertexBuffer(vertexBuffer);

		//Index Buffer
		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices)/sizeof(uint32_t)));
		_VertexArray->SetIndexBuffer(indexBuffer);

		_SquareVA.reset(VertexArray::Create());

		float vertices2[4 * 3] = {
			-0.75f, -0.75f, 0.0f,
			0.75f, -0.75f, 0.0f,
			0.75f, 0.75f, 0.0f,
			-0.75f, 0.75f, 0.0f,
		};

		std::shared_ptr<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::Create(vertices2, sizeof(vertices2)));
		squareVB->SetLayout({
			{ShaderDataType::Float3, "a_Position"},
			});
		_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<IndexBuffer> squareIB;
		squareIB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
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
		_Shader.reset(new Shader(vertexSrc, fragmentSrc));
		_Shader2.reset(new Shader(vertexSrc2, fragmentSrc2));

	}

	Application::~Application() {

	}
	void Application::Run() {
		while (bRunning) {

			RenderCommand::SetClearColor({ 0.4f, 0.3f, 0.3f, 1.0f });
			RenderCommand::Clear();

			//Beginning of the scene where we tell renderer what to do for the scene
			Renderer::BeginScene(_Camera);

			Renderer::Submit(_Shader2, _SquareVA);
			Renderer::Submit(_Shader, _VertexArray);

			//EndScene tells the renderer we are done submitting and it can do its thing now
			Renderer::EndScene();
			//flush the render queue
			//Renderer::Flush();

			_LayerManager.OnUpdate();

			_ImguiLayer->Begin();
			_LayerManager.OnImguiRender();
			_ImguiLayer->End();

			_Window->OnUpdate();
		}
	}

	void Application::OnWindowCloseEvent() {
		bRunning = false;
	}

	void Application::PushLayer(Layer *layer) {
		_LayerManager.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer *overlay) {
		_LayerManager.PushOverlay(overlay);
		overlay->OnAttach();
	}
}