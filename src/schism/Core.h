#pragma once

#ifdef SCHISM_PLATFORM_WINDOWS
	#ifdef SCHISM_BUILD_DLL
		#define SCHISM_API __declspec(dllexport)
	#else
		#define SCHISM_API __declspec(dllimport)
	#endif
#else
	// Error should be shown here I'm having issues
#endif