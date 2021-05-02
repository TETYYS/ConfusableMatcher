#pragma once

#include <codecvt>
#include <shared_mutex>
#include <unordered_set>
#include <vector>
#include <cstring>
#include <bitset>

#include "Config.h"
#include "Shared.h"

#ifdef WIN32
	#include <string_view>
	typedef std::string_view CMStringView;
#else
	#ifdef HAVE_EXPERIMENTAL_STRING_VIEW
		#include <experimental/string_view>
		typedef std::experimental::string_view CMStringView;
	#else
		#include <string_view>
		typedef std::string_view CMStringView;
	#endif
#endif

#ifdef WIN32
	#define HASHCOMPARE_CLS stdext::hash_compare
#else
	#define HASHCOMPARE_CLS std::hash
#endif

namespace google
{
	template<class T>
		class libc_allocator_with_realloc;

	template<class Key, class T, class HashFcn, class EqualKey, class Alloc>
		class dense_hash_map;
};

struct MatchingState
{
	CMStringView In;
	CMStringView Contains;
	int StartingIndex;
	int MatchedChars;
	CMStringView LastMatched;

	MatchingState(CMStringView In, CMStringView Contains, int StartingIndex, int MatchedChars, CMStringView LastMatched)
	{
		this->In = In;
		this->Contains = Contains;
		this->StartingIndex = StartingIndex;
		this->MatchedChars = MatchedChars;
		this->LastMatched = LastMatched;
	}
};

constexpr int STACKVECTOR_SIZE = 64;
template<class T>
struct StackVector
{
	int CurSize = 0;
	bool IsStack = true;

public:
	T Stack[STACKVECTOR_SIZE];
	std::vector<T> Heap;

	void Push(T In)
	{
		if (IsStack) {
			if (CurSize + 1 > STACKVECTOR_SIZE) {
				IsStack = false;
				Heap = std::vector<T>(&Stack[0], &Stack[STACKVECTOR_SIZE]);
				CurSize = 0;
			} else {
				Stack[CurSize++] = In;
				return;
			}
		}
		Heap.push_back(In);
	}

	void Reset()
	{
		if (IsStack)
			CurSize = 0;
		else
			Heap.clear();
	}

	size_t Size()
	{
		return IsStack ? CurSize : Heap.size();
	}
};

struct CMString {
	char *Str;
	size_t Len;

public:
	CMString()
	{
	}

	CMString(char *str, size_t len)
	{
		Str = str;
		Len = len;
	}

	static CMString CopyFromString(std::string In)
	{
		auto len = In.size();
		char *value = new char[len + 1];
		memcpy(value, In.c_str(), len + 1);

		return CMString(value, len);
	}

	CMStringView View()
	{
		return CMStringView(Str, Len);
	}

	void Free()
	{
		delete Str;
	}
};

class ConfusableMatcher
{
	private:

	typedef google::dense_hash_map<
		char, // Value first char
		std::vector<CMString>*, // Values whole
		HASHCOMPARE_CLS<char>,
		std::equal_to<char>,
		google::libc_allocator_with_realloc<std::pair<const char, std::vector<CMString>*>>
	> CMInnerHashMap;

	google::dense_hash_map<
		char, // Key first char
		std::vector<
			std::pair<
				CMString, // Key whole
				CMInnerHashMap*
			>*
		>*,
		HASHCOMPARE_CLS<char>,
		std::equal_to<char>,
		google::libc_allocator_with_realloc<std::pair<const char, std::vector<std::pair<CMString, CMInnerHashMap*>*>*>>
	>
		*TheMap;

	google::dense_hash_map<
		char,
		std::vector<CMString>*,
		HASHCOMPARE_CLS<char>,
		std::equal_to<char>,
		google::libc_allocator_with_realloc<std::pair<const char, std::vector<CMString>*>>
	>
		*SkipSet;
	
	static std::bitset<1114110> WordBoundaries;
	static bool Initialized;

	void Init();
	void GetMappings(CMStringView Key, CMStringView Value, StackVector<std::pair<CMStringView, CMStringView>> &Storage);
	CMReturn IndexOfFromView(CMStringView In, CMStringView Contains, CMOptions Options);
	CMReturn IndexOfInner(const CMStringView FullIn, MatchingState State, size_t *StatePushes, const CMOptions Options);
	CM_RETURN_STATUS CheckWordBoundary(CMStringView In, CMStringView Match);
	int MatchWordBoundary(unsigned char i0);
	int MatchWordBoundary(unsigned char i0, unsigned char i1);
	int MatchWordBoundary(unsigned char i0, unsigned char i1, unsigned char i2);
	int MatchWordBoundary(unsigned char i0, unsigned char i1, unsigned char i2, unsigned char i3);

	public:
	size_t MatchWordBoundaryToRight(CMStringView In);
	bool MatchWordBoundaryToLeft(CMStringView In);
	bool AddMapping(std::string Key, std::string Value, bool CheckValueDuplicate);
	bool AddSkip(std::string In);
	ConfusableMatcher(std::vector<std::pair<std::string, std::string>> InputMap, std::unordered_set<std::string> Skip, bool AddDefaultValues = true);
	CMReturn IndexOf(std::string In, std::string Contains, CMOptions Options);
	void GetKeyMappings(std::string In, StackVector<CMString> &Output);

	~ConfusableMatcher();
};