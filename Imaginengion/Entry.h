#pragma once

#ifdef IMAGINE_PLATFORM_WINDOWS

extern IM::Application* IM::CreateApplication();

int main(int argc, char** argv) {
	auto app = IM::CreateApplication();
	app->Run();
	delete app;
	return 0;
}
#else
	#error Imaginengion Only supports Windows !
#endif