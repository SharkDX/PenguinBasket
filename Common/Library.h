#pragma once

#ifdef COMMON_EXPORTS
	#define COMMON_LIBRARY __declspec(dllexport)
#else
	#define COMMON_LIBRARY __declspec(dllimport)
#endif