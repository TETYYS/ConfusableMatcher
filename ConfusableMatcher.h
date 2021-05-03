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

namespace google
{
	template<class T>
		class libc_allocator_with_realloc;

	template<class Key, class T, class HashFcn, class EqualKey, class Alloc>
		class dense_hash_map;
};

struct MatchingState
{
	size_t InPos;
	size_t ContainsPos;
	std::pair<size_t, size_t> LastMatched;

	MatchingState(size_t InPos, size_t ContainsPos, std::pair<size_t, size_t> LastMatched)
	{
		this->InPos = InPos;
		this->ContainsPos = ContainsPos;
		this->LastMatched = LastMatched;
	}
};

constexpr int STACKVECTOR_SIZE = 64;
template<class T>
struct StackVector
{
private:
	int CurSize = 0;
	T Stack[STACKVECTOR_SIZE];
	std::vector<T> Heap;

public:
	StackVector() { }

	inline T GetElement(size_t Pos)
	{
		return Pos < STACKVECTOR_SIZE ? Stack[Pos] : Heap[Pos - STACKVECTOR_SIZE];
	}

	void Push(T In)
	{
		if (CurSize + 1 > STACKVECTOR_SIZE) {
			Heap.push_back(In);
			CurSize++;
		} else {
			Stack[CurSize++] = In;
		}
	}

	void Reset()
	{
		if (CurSize > STACKVECTOR_SIZE)
			Heap.clear();
		CurSize = 0;
	}

	inline size_t Size()
	{
		return CurSize;
	}
};

struct CMString {
	char *Str;
	size_t Len;
	size_t Hash;

public:
	CMString()
	{
	}

	CMString(char *str, size_t len)
	{
		Str = str;
		Len = len;

		Hash = 14695981039346656037u;
		for (auto x = 0;x < Len;x++)
		{
			Hash ^= Str[x];
			Hash *= 1099511628211u;
		}
	}

	static CMString CopyFromString(std::string In)
	{
		auto len = In.size();
		char *value = new char[len + 1];
		memcpy(value, In.c_str(), len + 1);

		return CMString(value, len);
	}

	inline CMStringView View() const
	{
		return CMStringView(Str, Len);
	}

	void Free()
	{
		delete Str;
	}
};

struct eqstr
{
	bool operator()(CMString s1, CMString s2) const
	{
		return s1.Len == s2.Len && strncmp(s1.Str, s2.Str, s1.Len) == 0;
	}
};

struct hashstr
{
	size_t operator()(const CMString In) const
	{
		return In.Hash;
	}
};

typedef google::dense_hash_map<
	char, // Value first char
	std::vector<CMString>*, // Values whole
	std::hash<char>,
	std::equal_to<char>,
	google::libc_allocator_with_realloc<std::pair<const char, std::vector<CMString>*>>
> CMInnerHashMap;

struct CMStringPosPointers
{
	private:

	public:
	std::vector<std::vector<std::pair<size_t /* Key len */, const CMInnerHashMap*>>> PosPointers;
};

class ConfusableMatcher
{
	private:
	std::vector<CMString>* SkipSet[256];
	
	static std::bitset<1114110> WordBoundaries;
	static bool Initialized;

	void Init();
	void GetMappings(CMStringPosPointers *PosPointers, size_t Pos, long long ExactSize, CMStringView Value, size_t ValuePos, StackVector<std::pair<size_t, size_t>> &Storage);
	void GetMappings(CMStringView Key, size_t KeyPos, CMStringView Value, size_t ValuePos, StackVector<std::pair<size_t, size_t>> &Storage);
	CMReturn IndexOfFromView(CMStringView In, CMStringView Contains, CMOptions Options);
	CMReturn IndexOfInner(const CMStringView In, const CMStringView Contains, int StartingIndex, MatchingState State, size_t *StatePushes, const CMOptions Options);
	CM_RETURN_STATUS CheckWordBoundary(CMStringView In, CMStringView Match);
	int MatchWordBoundary(unsigned char i0);
	int MatchWordBoundary(unsigned char i0, unsigned char i1);
	int MatchWordBoundary(unsigned char i0, unsigned char i1, unsigned char i2);
	int MatchWordBoundary(unsigned char i0, unsigned char i1, unsigned char i2, unsigned char i3);

	public:

	google::dense_hash_map<
		char, // Key first char
		std::vector<
			std::pair<
				CMString, // Key whole
				CMInnerHashMap*
			>*
		>*,
		std::hash<char>,
		std::equal_to<char>,
		google::libc_allocator_with_realloc<std::pair<const char, std::vector<std::pair<CMString, CMInnerHashMap*>*>*>>
	>
		*TheMap;

	size_t MatchWordBoundaryToRight(CMStringView In);
	bool MatchWordBoundaryToLeft(CMStringView In);
	bool AddMapping(std::string Key, std::string Value, bool CheckValueDuplicate);
	bool AddSkip(std::string In);
	ConfusableMatcher(std::vector<std::pair<std::string, std::string>> InputMap, std::unordered_set<std::string> Skip, bool AddDefaultValues = true);
	CMReturn IndexOf(std::string In, std::string Contains, CMOptions Options);
	void GetKeyMappings(std::string In, StackVector<CMString> &Output);
	CMStringPosPointers *ComputeStringPosPointers(std::string Contains);

	~ConfusableMatcher();
};