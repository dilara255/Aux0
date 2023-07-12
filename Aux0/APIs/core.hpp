#pragma once

//Definições para controle dos projetos, sistemas, compilação, linkagem, etc
//TODO: This could all be on the build system. Is that preferable? 
//Either way, go all the way EITHER there OR here

#ifndef AUX0
	#include "logAPI.hpp"
#endif

#define BIT(x) (1 << x)
namespace AZ{ long long getExpectedWakeUpDelay(long long microsToSleep); }

#ifdef AS_PLATFORM_WINDOWS

	#define	API_EXPORT __declspec(dllexport)
	#define	API_IMPORT __declspec(dllimport)

	#define AZ_LOG_TRACE_COLOR (FOREGROUND_BLUE | FOREGROUND_GREEN)
	#define AZ_LOG_DEBUG_COLOR (FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY)
	#define AZ_LOG_INFO_COLOR (FOREGROUND_GREEN | FOREGROUND_INTENSITY)
	
	//Based on tests on a single machine, tuned for release mode with low concurrency
	inline long long AZ::getExpectedWakeUpDelay(long long microsToSleep) {
		return (long long)round(478 - 0.00373*microsToSleep);
	}

#else
	#error no support for other platforms as of yet : /
#endif

/* These have all been moved to the build system

#ifdef AS_PLATFORM_WINDOWS
	#define SYSTEM_NAME "Windows" //Moved to the build system
#endif

#ifdef AS_DEBUG
	#define CONFIG_NAME "Debug"
#elif RELEASE 
	#define CONFIG_NAME "Release"
#endif

#ifdef X64
	#define PLATFORM_NAME "64 bits"
#elif X86
	#define PLATFORM_NAME "32 bits"
#endif

*/




