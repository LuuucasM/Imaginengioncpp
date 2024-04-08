#include "impch.h"
#include "Application.h"

#include "Renderer/Renderer.h"

#include "glad/glad.h"

namespace IM {

	Application* Application::_Instance = nullptr;

	Application::Application(const std::string& name) {
		IMAGINE_PROFILE_FUNCTION();

		//Application is singleton so check it does not already exist
		IMAGINE_CORE_ASSERT(!_Instance, "Application already exists!");
		_Instance = this;
		IMAGINE_CORE_TRACE("number of possible threads: {}", _CPUProfiler.GetThreadCount());
		//create window and bind windowcloseevent to application function
		_Window = Window::Create(WindowProps(name));
		_Window->SetEventCallback(IMAGINE_BIND_EVENT(Application::OnEvent));
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
	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(IMAGINE_BIND_EVENT(OnWindowCloseEvent));
		dispatcher.Dispatch<WindowResizeEvent>(IMAGINE_BIND_EVENT(OnWindowResizeEvent));

		for (auto it = _LayerManager.rbegin(); it != _LayerManager.rend(); ++it) {
			if (e._bHandled) {
				break;
			}
			(*it)->OnEvent(e);
		}
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

	bool Application::OnWindowCloseEvent(WindowCloseEvent& e)
	{
		_bRunning = false;
		return true;
	}

	bool Application::OnWindowResizeEvent(WindowResizeEvent& e)
	{

		IMAGINE_PROFILE_FUNCTION();

		if (e.GetWidth() == 0 || e.GetHeight() == 0) {
			_bMinimized = true;
			return false;
		}
		_bMinimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());
		return false;
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