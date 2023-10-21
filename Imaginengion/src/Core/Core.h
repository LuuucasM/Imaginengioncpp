#pragma once


#ifdef IMAGINE_PLATFORM_WINDOWS
	#ifdef IMAGINE_BUILD_DLL
		#define IMAGINE_API __declspec(dllexport)
	#else
		#define IMAGINE_API __declspec(dllimport)
	#endif
#else
	#error Imaginengion Only supports Windows !
#endif

#ifdef IMAGINE_ENABLE_ASSERTS
	#define IMAGINE_ASSERT(x, ...) { if(!(x)) {IMAGINE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define IMAGINE_CORE_ASSERT(x, ...) { if(!(x)) {IMAGINE_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define IMAGINE_ASSERT(x, ...)
	#define IMAGINE_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)