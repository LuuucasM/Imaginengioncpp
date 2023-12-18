#include "impch.h"
#include "Application.h"

#include "Renderer/Renderer.h"

#include "glad/glad.h"

namespace IM {

	Application* Application::_Instance = nullptr;

	Application::Application() {
		IMAGINE_PROFILE_FUNCTION();

		//Application is singleton so check it does not already exist
		IMAGINE_CORE_ASSERT(!_Instance, "Application already exists!");
		_Instance = this;

		//create window and bind windowcloseevent to application function
		_Window = ScopePtr<Window>(Window::Create());
		_Window->WindowCloseEvent.AddListener(this, &Application::OnWindowCloseEvent);
		_Window->WindowResizeEvent.AddListener(this, &Application::OnWindowResizeEvent);
		_Window->SetVSync(false);

		Renderer::Init();

		_ImguiLayer = new ImguiLayer();
		PushOverlay(_ImguiLayer);

		_Timestep.Start();
	}

	Application::~Application() {

		IMAGINE_PROFILE_FUNCTION();

		Renderer::Shutdown();
	}
	void Application::Run() {

		IMAGINE_PROFILE_FUNCTION();

		while (_bRunning) {

			IMAGINE_PROFILE_SCOPE("Run Loop - Application::Run()");

			float delta_time = _Timestep.GetDeltaTimeInSec();

			if (!_bMinimized) {

				{
					IMAGINE_PROFILE_SCOPE("Layer Manager OnUpdate - Application::Run()");
					_LayerManager.OnUpdate(delta_time);
				}
				_ImguiLayer->Begin();
				{
					IMAGINE_PROFILE_SCOPE("Layer Manager OnImGuiRender - Application::Run()");
					_LayerManager.OnImguiRender();
				}
				_ImguiLayer->End();
			}

			_Window->OnUpdate();
		}
	}

	void Application::OnWindowCloseEvent() {
		_bRunning = false;
	}

	void Application::OnWindowResizeEvent(int width, int height)
	{

		IMAGINE_PROFILE_FUNCTION();

		if (width == 0 || height == 0) {
			_bMinimized = true;
			return;
		}
		_bMinimized = false;
		Renderer::OnWindowResize(width, height);
	}

	void Application::PushLayer(Layer *layer) {

		IMAGINE_PROFILE_FUNCTION();

		_LayerManager.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer *overlay) {

		IMAGINE_PROFILE_FUNCTION();

		_LayerManager.PushOverlay(overlay);
		overlay->OnAttach();
	}
}