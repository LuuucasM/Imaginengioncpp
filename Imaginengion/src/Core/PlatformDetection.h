#pragma once
#ifdef _WIN32
	#ifdef _WIN64
			#define IMAGINE_PLATFORM_WINDOWS
			#define IMAGINE_OPENGL
		#else
			#error "x86 Builds are not supported!"
	#endif
#elif defined(__APPLE__) || defined(__MACH__)
	#include <TargetConditionals.h>
	#if TARGET_IPHONE_SIMULATOR == 1
		#error "IOS simulator is not supported!"
	#elif TARGET_OS_IPHONE == 1
		#define IMAGINE_PLATFORM_IOS
		#define IMAGINE_OPENGL
		#error "IOS is not supported!"
	#elif TARGET_OS_MAC == 1
		#define IMAGINE_PLATFORM_MACOS
		#define IMAGINE_OPENGL
		#error "MacOS is not supported!"
	#else
		#define "Unknown Apple platform!"
	#endif
#elif defined(__ANDRIOD__)
	#define IMAGINE_PLATFORM_ANDRIOD
	#define IMAGINE_OPENGL
	#error "Andriod is not supported!"
#elif defined(__linux__)
	#define IMAGINE_PLATFORM_LINUX
	#define IMAGINE_OPENGL
	#error "Linux is not supported!"
#else
	#error "Unknown platform!"
#endif