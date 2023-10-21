#include "impch.h"
#include "Application.h"

namespace IM {
	Application::Application() {
		_Window = std::unique_ptr<Window> (Window::Create());
		_Window->WindowCloseEvent.AddListener<Application>(this, &Application::OnWindowCloseEvent);
	}

	Application::~Application() {

	}
	void Application::Run() {
		while (bRunning) {
			_Window->OnUpdate();
		}
	}

	void Application::OnWindowCloseEvent() {
		bRunning = false;
	}
}