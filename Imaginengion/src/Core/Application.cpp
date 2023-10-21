#include "impch.h"
#include "Application.h"

namespace IM {
	Application::Application() {
		_Window = std::unique_ptr<Window> (Window::Create());
	}

	Application::~Application() {

	}

	void Application::Run() {
		while (bRunning) {
			_Window->OnUpdate();
		}
	}
}