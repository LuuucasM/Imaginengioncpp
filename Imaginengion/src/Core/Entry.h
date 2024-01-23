#pragma once

#include "Application.h"

#ifdef IMAGINE_PLATFORM_WINDOWS

extern IM::Application* IM::CreateApplication();

int main(int argc, char** argv) {

	IM::Log::Init();

	IMAGINE_PROFILE_BEGIN_SESSION("Startup", "ImaginePerfProfile-Startup.json");
	auto app = IM::CreateApplication();
	IMAGINE_PROFILE_END_SESSION();

	IMAGINE_PROFILE_BEGIN_SESSION("Runtime", "ImaginePerfProfile-Runtime.json");
	app->Run();
	IMAGINE_PROFILE_END_SESSION();

	IMAGINE_PROFILE_BEGIN_SESSION("Shutdown", "ImaginePerfProfile-Shutdown.json");
	delete app;
	IMAGINE_PROFILE_END_SESSION();

	return 0;
}
#else
	#error Imaginengion Only supports Windows !
#endif