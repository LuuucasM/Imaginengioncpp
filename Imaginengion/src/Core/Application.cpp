#include "impch.h"
#include "Application.h"

#include <GLFW/glfw3.h>

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
			_Window->OnUpdate();
		}
	}

	void Application::OnWindowCloseEvent() {
		bRunning = false;
	}
}