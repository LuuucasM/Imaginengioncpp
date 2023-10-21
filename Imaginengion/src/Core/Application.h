#pragma once

#include "Core.h"
#include "Window.h"
#include "Log.h"

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
	private:
		bool bRunning = true;
	};

	//to be defined in client
	Application* CreateApplication();
}

