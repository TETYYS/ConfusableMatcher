#pragma once

#ifdef __cplusplus
	#include <cstdint>
#else
	#include <stdint.h>
	#include <stdbool.h>
#endif

#if defined _WIN32 || defined __CYGWIN__
	#ifdef __GNUC__
		#define EXPORTED __attribute__ ((dllimport))
		#define __cdecl __attribute__((cdecl))
	#else
		#define EXPORTED __declspec(dllimport)
	#endif
#else
	#define __cdecl __attribute__((cdecl))
	#if __GNUC__ >= 4
		#define EXPORTED __attribute__ ((visibility ("default")))
	#else
		#define EXPORTED
	#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif
	typedef void *CMHandle;
	typedef void *CMListHandle;
	typedef void *CMComputedStringPosPointersHandle;

	typedef struct _CMKV
	{
		char *Key;
		char *Value;
	} CMKV;

	typedef struct _CMMap
	{
		CMKV *Kv;
		uint32_t Size;
	} CMMap;

#include "Shared.h"

	EXPORTED CMHandle __cdecl InitConfusableMatcher(CMMap Map, char** IgnoreList, int IgnoreCount, bool AddDefaultValues);
	EXPORTED void __cdecl FreeConfusableMatcher(CMHandle In);
	EXPORTED CMReturn __cdecl StringIndexOf(CMHandle CM, char* In, char* Contains, CMOptions Options);
	EXPORTED CMReturn StringIndexOfDebugFailures(CMHandle CM, char* In, char* Contains, CMOptions Options, CMDebugFailures* DebugOut);
	EXPORTED void FreeDebugFailures(CMDebugFailures* DebugFailures);
	EXPORTED uint32_t __cdecl GetKeyMappings(CMHandle CM, char* In, const char** Output, uint32_t OutputSize);
	EXPORTED CMComputedStringPosPointersHandle __cdecl ComputeStringPosPointers(CMHandle CM, char *Contains);
	EXPORTED void __cdecl FreeStringPosPointers(CMComputedStringPosPointersHandle StringPosPointers);
#ifdef __cplusplus
}
#endif