#pragma once

#include <cstdint>

#if defined _WIN32 || defined __CYGWIN__
	#ifdef WIN_EXPORT
		#ifdef __GNUC__
			#define EXPORTED __attribute__ ((dllexport))
		#else
			#define EXPORTED __declspec(dllexport)
		#endif
	#else
		#ifdef __GNUC__
			#define EXPORTED __attribute__ ((dllimport))
		#else
			#define EXPORTED __declspec(dllimport)
		#endif
	#endif
	#define NOT_EXPORTED
#else
	#if __GNUC__ >= 4
		#define EXPORTED __attribute__ ((visibility ("default")))
		#define NOT_EXPORTED  __attribute__ ((visibility ("hidden")))
	#else
		#define EXPORTED
		#define NOT_EXPORTED
	#endif
#endif

extern "C" {

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

	EXPORTED CMHandle InitConfusableMatcher(CMMap Map, bool AddDefaultValues);
	EXPORTED void FreeConfusableMatcher(CMHandle In);
	EXPORTED CMListHandle ConstructIgnoreList(char **List, int Count);
	EXPORTED void FreeIgnoreList(CMListHandle List);
	EXPORTED uint64_t StringIndexOf(CMHandle CM, char *In, char *Contains, bool MatchRepeating, int StartIndex, CMListHandle IgnoreList);
	EXPORTED bool AddMapping(CMHandle CM, char *Key, char *Value, bool CheckValueDuplicate);
	EXPORTED bool RemoveMapping(CMHandle CM, char *Key, char *Value);
}