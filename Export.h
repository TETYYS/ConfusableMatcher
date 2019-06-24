#pragma once

#include <cstdint>

extern "C" {

	typedef void *CMHandle, *CMListHandle;

	typedef struct _CMKV
	{
		char *Key;
		char *Value;
	} CMKV;

	typedef struct _CMMap
	{
		CMKV *Kv;
		size_t Size;
	} CMMap;

	__declspec(dllexport) CMHandle InitConfusableMatcher(CMMap Map, bool AddDefaultValues);
	__declspec(dllexport) CMListHandle ConstructIgnoreList(char **List, int Count);
	__declspec(dllexport) void FreeIgnoreList(CMListHandle List);
	__declspec(dllexport) uint64_t StringIndexOf(CMHandle CM, char *In, char *Contains, bool MatchRepeating, int StartIndex, CMListHandle IgnoreList);
	__declspec(dllexport) bool AddMapping(CMHandle CM, char *Key, char *Value, bool CheckValueDuplicate);
	__declspec(dllexport) bool RemoveMapping(CMHandle CM, char *Key, char *Value);
}