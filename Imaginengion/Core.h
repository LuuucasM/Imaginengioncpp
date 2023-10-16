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