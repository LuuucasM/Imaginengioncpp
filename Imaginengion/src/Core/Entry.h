#pragma once

#include "../Imagine.h"

#ifdef IMAGINE_PLATFORM_WINDOWS

extern IM::Application* IM::CreateApplication();

int main(int argc, char** argv) {

	IM::Log::Init();
	IMAGINE_CORE_WARN("random message AAAAAAA");
	IMAGINE_CLIENT_INFO("GOTTA GO FAST");
	IM::Event<int, int> myEvent(IM::EventType::MouseMoved, IM::EventCategory::EC_Mouse);
	if (myEvent.GetEventCategory() & IM::EventCategory::EC_Mouse) {
		IMAGINE_CORE_INFO("category is application");
	}
	auto app = IM::CreateApplication();
	app->Run();
	delete app;
	return 0;
}
#else
	#error Imaginengion Only supports Windows !
#endif