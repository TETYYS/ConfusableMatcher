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

	typedef struct _CMIntPair
	{
		int32_t First;
		int32_t Second;
	} CMIntPair;

	EXPORTED CMHandle __cdecl InitConfusableMatcher(CMMap Map, char** IgnoreList, int IgnoreCount, bool AddDefaultValues);
	EXPORTED void __cdecl FreeConfusableMatcher(CMHandle In);
	EXPORTED CMIntPair __cdecl StringIndexOf(CMHandle CM, char* In, char* Contains, bool MatchRepeating, int StartIndex, bool StartFromEnd, int StatePushLimit);
	EXPORTED uint32_t __cdecl GetKeyMappings(CMHandle CM, char* In, const char** Output, uint32_t OutputSize);
#ifdef __cplusplus
}
#endif