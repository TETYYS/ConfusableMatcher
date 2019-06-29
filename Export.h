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

	typedef enum _MAPPING_RESPONSE
	{
		SUCCESS = 0,
		ALREADY_EXISTS = 1,
		EMPTY_KEY = 2,
		EMPTY_VALUE = 3,
		INVALID_KEY = 4,
		INVALID_VALUE = 5
	} MAPPING_RESPONSE;

	EXPORTED CMHandle __cdecl InitConfusableMatcher(CMMap Map, bool AddDefaultValues);
	EXPORTED void __cdecl FreeConfusableMatcher(CMHandle In);
	EXPORTED CMListHandle __cdecl ConstructIgnoreList(char **List, int Count);
	EXPORTED void __cdecl FreeIgnoreList(CMListHandle List);
	EXPORTED uint64_t __cdecl StringIndexOf(CMHandle CM, char *In, char *Contains, bool MatchRepeating, int StartIndex, CMListHandle IgnoreList);
	EXPORTED MAPPING_RESPONSE __cdecl AddMapping(CMHandle CM, char *Key, char *Value, bool CheckValueDuplicate);
	EXPORTED bool __cdecl RemoveMapping(CMHandle CM, char *Key, char *Value);
#ifdef __cplusplus
}
#endif