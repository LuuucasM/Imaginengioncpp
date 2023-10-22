#pragma once

#include "Core.h"
#include "Window.h"
#include "LayerManager.h"

#include <memory>

namespace IM {

	class IMAGINE_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		std::unique_ptr<Window> _Window;
		void OnWindowCloseEvent();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
	private:
		bool bRunning = true;

		LayerManager _LayerManager;
	};

	//to be defined in client
	Application* CreateApplication();
}

