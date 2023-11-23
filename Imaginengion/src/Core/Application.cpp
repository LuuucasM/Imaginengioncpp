#include "impch.h"
#include "Application.h"

namespace IM {

	Application* Application::_Instance = nullptr;

	Application::Application() {
		//Application is singleton so check it does not already exist
		IMAGINE_CORE_ASSERT(!_Instance, "Application already exists!");
		_Instance = this;

		//create window and bind windowcloseevent to application function
		_Window = std::unique_ptr<Window>(Window::Create());
		_Window->WindowCloseEvent.AddListener<Application>(this, &Application::OnWindowCloseEvent);

		_ImguiLayer = new ImguiLayer();
		PushOverlay(_ImguiLayer);
		_Timestep.Start();
	}

	Application::~Application() {

	}
	void Application::Run() {
		while (bRunning) {

			float delta_time = _Timestep.GetDeltaTimeInSec();

			_LayerManager.OnUpdate(delta_time);

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