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

		static void testFunc(int width, int height) {
			IMAGINE_CORE_INFO("this is a test to see if event system adds correctly!");
		}
	private:
		
		bool bRunning = true;
	};

	//to be defined in client
	Application* CreateApplication();
}

