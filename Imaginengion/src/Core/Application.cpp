#include "impch.h"
#include "Application.h"

#include <glad/glad.h>

namespace IM {

	Application* Application::_Instance = nullptr;

	Application::Application() {
		//Application is singleton so check it does not already exist
		IMAGINE_CORE_ASSERT(!_Instance, "Application already exists!");
		_Instance = this;

		//create window and bind windowcloseevent to application function
		_Window = std::unique_ptr<Window> (Window::Create());
		_Window->WindowCloseEvent.AddListener<Application>(this, &Application::OnWindowCloseEvent);

		_ImguiLayer = new ImguiLayer();
		PushOverlay(_ImguiLayer);

		//FOR MAKING A TRIANGLE--------------
		//Vertex Array
		glGenVertexArrays(1, &_VertexArray);
		glBindVertexArray(_VertexArray);

		//Vertex Buffer
		glGenBuffers(1, &_VertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, _VertexBuffer);

		float vertices[9] = {
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.0f, 0.5f, 0.0f
		};

		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, nullptr);
		//Index Buffer
		glGenBuffers(1, &_IndexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _IndexBuffer);

		unsigned int indecies[3] = { 0, 1, 2 };
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indecies), indecies, GL_STATIC_DRAW);

		//TEMPORARY------------------
		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			out vec3 v_Position;			

			void main() {
				v_Position = a_Position;
				gl_Position = vec4(a_Position, 1.0);
			}
		)";
		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;		

			void main() {
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
			}
		)";
		_Shader.reset(new Shader(vertexSrc, fragmentSrc));

	}

	Application::~Application() {

	}
	void Application::Run() {
		while (bRunning) {
			glClearColor(0.4f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			_Shader->Bind();
			glBindVertexArray(_VertexArray);
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

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