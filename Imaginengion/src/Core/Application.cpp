#include "impch.h"
#include "Application.h"

#include <glad/glad.h>

namespace IM {
	Application::Application() {
		_Window = std::unique_ptr<Window> (Window::Create());
		_Window->WindowCloseEvent.AddListener<Application>(this, &Application::OnWindowCloseEvent);
	}

	Application::~Application() {

	}
	void Application::Run() {
		while (bRunning) {
			glClearColor(0.4, 0.3, 0.3, 1.0);
			glClear(GL_COLOR_BUFFER_BIT);

			_LayerManager.OnUpdate();

			_Window->OnUpdate();
		}
	}

	void Application::OnWindowCloseEvent() {
		bRunning = false;
	}

	void Application::PushLayer(Layer *layer) {
		_LayerManager.PushLayer(layer);
	}

	void Application::PushOverlay(Layer *overlay) {
		_LayerManager.PushOverlay(overlay);
	}
}