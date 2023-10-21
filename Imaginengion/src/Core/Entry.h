#pragma once

#include "../Imagine.h"

#ifdef IMAGINE_PLATFORM_WINDOWS

extern IM::Application* IM::CreateApplication();

int main(int argc, char** argv) {

	IM::Log::Init();
	IMAGINE_CORE_WARN("random message AAAAAAA");
	IMAGINE_INFO("GOTTA GO FAST");
	auto app = IM::CreateApplication();
	app->_Window->WindowResizeEvent.AddListener(app, IM::Application::testFunc);
	app->Run();
	delete app;
	return 0;
}
#else
	#error Imaginengion Only supports Windows !
#endif