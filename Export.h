#pragma once

#include <cstdint>

extern "C" {

	typedef void *CMHandle, *CMListHandle;

	typedef struct _CMKV
	{
		uint32_t Key;
		char *Value;
	} CMKV;

	typedef struct _CMMap
	{
		CMKV *Kv;
		size_t Size;
	} CMMap;

	__declspec(dllimport) CMHandle InitConfusableMatcher(CMMap Map);
	__declspec(dllimport) CMListHandle ConstructIgnoreList(uint32_t *List, int Count);
	__declspec(dllimport) void FreeIgnoreList(CMListHandle List);
	__declspec(dllimport) uint64_t StringIndexOf(CMHandle CM, char *In, char *Contains, bool MatchRepeating, int StartIndex, CMListHandle IgnoreList);
	__declspec(dllimport) bool AddMapping(CMHandle CM, uint32_t Key, char *Value, bool CheckValueDuplicate);
	__declspec(dllimport) bool RemoveMapping(CMHandle CM, uint32_t Key, char *Value);
}