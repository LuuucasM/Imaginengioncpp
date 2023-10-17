#pragma once

#include "Core.h"

namespace IM {

	class IMAGINE_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	//to be defined in client
	//comment
	Application* CreateApplication();
}

